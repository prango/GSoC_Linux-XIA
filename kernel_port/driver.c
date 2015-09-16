#include<xia_vxidty.h>
#include<vxidty.c>


int main(int argc, char * argv[]){


    B=(struct xip_vxt_entry *)malloc(sizeof(struct xip_vxt_entry)*2048);
    
    //vxt_register_xidty(12,45,1);
    //vxt_register_xidty(13,47,1);
    //vxt_register_xidty(512+12,49,1);
    int i;
    for(i=12;i<48;i++) vxt_register_xidty(i,56,1);
    vxt_register_xidty(512+12,49,1);
    //Testing add and remove funtion.---------------- 
    printf("key = %d\n data = %d,",B[12].key,B[12].xid_type);
    printf("before vxt_unregister_xidty =%d\n",xt_to_vxt_rcu(12));
    vxt_unregister_xidty(12);
    printf("data value =%d\n,",B[12].xid_type);
    printf("after vxt_unregister_xidty =%d\n",xt_to_vxt_rcu(12));
    //------------------------------------------------------------
    printf("%d\n",xt_to_vxt_rcu(13));
    printf("%d\n",xt_to_vxt_rcu(14));
    printf("%d\n",xt_to_vxt_rcu(512+12));
    for(i=12;i<50;i++) printf("i = %d value %d\n",i,B[i].key);
    return 1;
}
