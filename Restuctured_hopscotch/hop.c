#include<stdio.h>
#include<stdlib.h>
<<<<<<< HEAD
//#include<net/xia.h>
=======
#include <linux/kernel.h>
//#include <linux/sockets.h>
#include <asm/byteorder.h>
/*#include "lookup3.c" //custum hash fuctions*/
/*-----------------------------------MACROS----------*/
>>>>>>> efc710b6da82a150cf79ab89071e77c93fb819d2

#define XIP_VXT_TABLE_SIZE 64 //XIP_VXT_TABLE_SIZE
#define HOP_RANGE 32
#define ADD_RANGE 256
int BUSY=-1;
typedef __be32 xid_type_t;

<<<<<<< HEAD
#define size 64 //size
#define HOP_RANGE = 32;
#define ADD_RANGE = 256; //geeting the full alphabet space for hexadecimal
=======
/*---------------------------------*/
>>>>>>> efc710b6da82a150cf79ab89071e77c93fb819d2

struct xip_vxt_entry{

    int key;
    xid_type_t xid_type;
   
    unsigned int hop_info;
    int lock;

};


struct xip_vxt_entry * B;



int Hash_Value(int);
int xt_to_vxt_rcu(int);
void resize();
void find_closer_bucket(struct xip_vxt_entry **,int *);
int vxt_unregister_xidty(xid_type_t);

/*static unsigned int Hash_Value(xid_type_t ty){
 return hash=((__be32_to_cpu(ty) & (XIP_VXT_TABLE_SIZE-1));
 } 
*/
void lock(struct xip_vxt_entry * bucket){

    
    while (1){
        if (!bucket->lock){
            if(!__sync_lock_test_and_set(&(bucket->lock),1)) break;
        }
    }
}

void unlock(struct xip_vxt_entry * bucket){

    bucket->lock = 0;
}

//int hash_fuction(int ty)

/*------- vxt_register_xidty ADD the xid_type in the hash tabel-----*/
int vxt_register_xidty(int key,xid_type_t xid_type,int reentrant){
    
    int hash=key%XIP_VXT_TABLE_SIZE;
    struct xip_vxt_entry * start_bucket = &B[hash];
    if (reentrant) lock(start_bucket);
    
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
        free_bucket=&B[temp_hash]; //danger
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
         }while( NULL!= free_bucket);
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
    *free_distance =0;


}


void resize(){
    //TODO: Not required when number of principal are limited TAble size can be incresed. After kernel port include code  
    printf(" @resize \n");
}

/*----------- This fuctions LOOKUP Xid_type int the maps and returns 0 if not found and 1 if found------*/
int xt_to_vxt_rcu(int key){
    
    int hash=key%XIP_VXT_TABLE_SIZE;
    struct xip_vxt_entry * start_bucket  = &B[hash];

    int try_counter = 0;
    int timestamp;

    //TODO:include the fast path too

    int i;
    int temp_hash=hash;
    struct xip_vxt_entry * check_bucket = start_bucket;
    for(i=0;i<HOP_RANGE;++i){
        
        if(key == (check_bucket->key))
            return 1;
        temp_hash++;
        check_bucket=&B[temp_hash];
    }
    return 0;
}


/*-----------------------------vxt_unregister_xidty function with REMOVES the xid @ty from the maps-----*/
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







int main(int argc, char * argv[]){


    B=(struct xip_vxt_entry *)malloc(sizeof(struct xip_vxt_entry)*2048);
    
    //vxt_register_xidty(12,45,1);
    //vxt_register_xidty(13,47,1);
    //vxt_register_xidty(512+12,49,1);
    int i;
    for(i=12;i<48;i++) vxt_register_xidty(i,56,1);
    vxt_register_xidty(512+12,49,1);
    printf("key = %d\n data = %d,",B[12].key,B[12].xid_type);
    printf("before vxt_unregister_xidty =%d\n",xt_to_vxt_rcu(12));
    vxt_unregister_xidty(12);
    printf("data value =%d\n,",B[12].xid_type);
    printf("after vxt_unregister_xidty =%d\n",xt_to_vxt_rcu(12));
    printf("%d\n",xt_to_vxt_rcu(13));
    printf("%d\n",xt_to_vxt_rcu(14));
    printf("%d\n",xt_to_vxt_rcu(512+12));
    for(i=12;i<50;i++) printf("i = %d value %d\n",i,B[i].key);
    return 1;
}
