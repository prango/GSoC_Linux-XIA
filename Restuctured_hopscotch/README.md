There are 3 main fuctions in this code :  
1. `xt_to_vxt_rcu(int)` -> LOOK_UP : this fuction looks up the kry in the hashed Buckets and Returns 0 if its not found and 1 If it get the key in the table  
2. `vxt_register_xidty(int key,xid_type_t xid_type,int lock)` -> ADD : this fuction adds the key value in the hash bucket.  
3.  `vxt_unregister_xidty(int key)` -> REMOVE : this fuctions remove the given key from the bicket and replaces them with zero.
