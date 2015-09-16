#ifndef _NET_XIA_VXIDTY
#define _NET_XIA_VXIDTY

#include <net/xia.h>

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
  
  

int vxt_register_xidty(xid_type_t ty);

int vxt_unregister_xidty(xid_type_t ty);

#endif
