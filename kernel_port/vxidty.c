#include <xia_vxidty.h>

/*------- vxt_register_xidty ADD the xid_type in the hash table-----*/



int vxt_register_xidty(int key,xid_type_t xid_type,int reentrant){
    
    int hash=key%XIP_VXT_TABLE_SIZE;
    struct xip_vxt_entry * start_bucket = &B[hash];
    if (reentrant) lock(start_bucket); //Lock Bucket
    
   
    if(xt_to_vxt_rcu(key)){
        if (reentrant) unlock(start_bucket);
        return 0;
    }

    struct xip_vxt_entry * free_bucket = start_bucket;
    int free_distance =0;
    int temp_hash=hash;
    for(free_distance=0;free_distance<ADD_RANGE; ++free_distance){
        if((0==free_bucket->key) &&(__sync_bool_compare_and_swap(&(free_bucket->key),0,BUSY))) //danger
            break;
        temp_hash++;
        free_bucket=&B[temp_hash];
     }
    
    if( free_distance <ADD_RANGE){
        do{
            if  (free_distance <HOP_RANGE){
                start_bucket->hop_info |=(1<< free_distance);
                free_bucket->xid_type = xid_type;
                free_bucket->key = key;
                if(reentrant) unlock(start_bucket);
                return 1;
             }
             find_closer_bucket(&free_bucket,&free_distance);
         }while( NULL != free_bucket);
    }
    if(reentrant)unlock(start_bucket);
    resize();
    return vxt_register_xidty(key,xid_type,0);
}

void find_closer_bucket(struct xip_vxt_entry **free_bucket, int * free_distance){


    struct xip_vxt_entry * move_bucket = *free_bucket -(HOP_RANGE -1);
    if (move_bucket<(&B[0])) move_bucket=&B[0];
    int free_dist;
    for(free_dist = (HOP_RANGE -1); free_dist>0;--free_dist){
        unsigned start_hop_info = move_bucket->hop_info;
        int move_free_distance =-1;
        unsigned int mask =1;
        int i;
        for(i=0;i<free_dist; ++i,mask<<=1){
            if (mask &start_hop_info){
                move_free_distance=i;
                break;
            }
        }
        if (-1!=move_free_distance){
            lock(move_bucket);
            if (start_hop_info ==  move_bucket->hop_info){
                struct xip_vxt_entry * new_free_bucket = move_bucket + move_free_distance;
                move_bucket->hop_info |= (1<<free_dist);
                (*free_bucket)->xid_type =new_free_bucket->xid_type;
                (*free_bucket)->key = new_free_bucket->key;
                //++(move_bucket->timestamp);
                new_free_bucket->key = BUSY;
                new_free_bucket->xid_type = BUSY;
                move_bucket->hop_info &=~(1<< move_free_distance);
                *free_bucket = new_free_bucket;
                *free_distance -=free_dist;
                unlock(move_bucket);
                return;
            }
            unlock(move_bucket);
        }
        move_bucket++;
    }
    *free_bucket = NULL; 
    *free_distance = 0;


}

/*-----------------------------vxt_unregister_xidty function with REMOVES the xid @ty used as key here  from the maps-----*/
int vxt_unregister_xidty(xid_type_t key){
    int hash=key%XIP_VXT_TABLE_SIZE;
    struct xip_vxt_entry * start_bucket  = &B[hash];
    lock(start_bucket);
    unsigned int hop_info = start_bucket->hop_info;
    unsigned int mask = 1;
    int i;
    for (i= 0;i< HOP_RANGE;++i,mask<<=1) {
      if(mask & hop_info){
  			struct xip_vxt_entry* check_bucket = start_bucket+i;
  			if(key==(check_bucket->key)){
  				int rc = check_bucket->xid_type;
  				check_bucket->key=0;
  				check_bucket->xid_type=0;
  				start_bucket->hop_info &=~(1<<i);
  				unlock(start_bucket);
  				return rc;
  			}
  		}
  	}
  	unlock(start_bucket);
  	return 0;
  }

