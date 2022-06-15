/*
 * service_priority_organizer.c
 *
 * Created: 22/11/2015 12:22:24 ص
 *  Author: ahmed
 */ 

#include <service_priority_organizer.h>
#include <requests.h>
#include <floor_counter.h>
#include <auto_state.h>
#include <e_setup.h>
#include <SEDACIO.h>
#include <control_status_register.h>


char direction_up;


int get_scheduled_floor(void)
{
	int temp;
	switch (collect)
	{
		case 0:
		temp = down_coll();
		break;
		case 1:
		temp = down_coll();
		break;
		case 2:
		temp = all_coll();
		break;
	}
	if(temp!=-1)
	{
		ar_up(direction_up);
		ar_dwn(!direction_up);
	}
	else if(mnt==0)
	{
		ar_up(0);
		ar_dwn(0);
	}
	return temp;
}
//char service_p_org_coll_checked = 0;
int down_coll()
{
    int nearest_int_floor,nearest_ext_floor,highest_ext_floor;
	//service_p_org_coll_checked = 0;
	if(get_current_floor() > f_max) 
	{
		selector_counter = 0;
		drop_all_req();
		return -1;	
	}
	
strt1:

    if((ireq|ereq)==0)//if there no request left exit this function
        return -1;
        
    else if(fw==1)
    {
        if(ireq==0)//if there no internal request left (case of full weight) exit this function
            return -1;
    }
    
    nearest_int_floor=get_nearest_floor(r_internal);
    if(fw==0)
    {
        nearest_ext_floor = get_nearest_floor(r_external);
        highest_ext_floor = get_highest_floor(r_dir_both);
    }
    else if(fw==1)// if the car in full weight don't respond to external hall calls
    {
        nearest_ext_floor=-1;
        highest_ext_floor=get_highest_floor(r_internal);
    }
    
strt:
    if(direction_up)    
    {
        //take only internal car calls
        if(nearest_int_floor!=-1)//there is internal request to serve
        {
            return nearest_int_floor; 
        }
        else // there is no internal request to serve
        {
            // check for highest external in up direction
            if(nearest_ext_floor!=-1)//there is external floor in that direction
                return highest_ext_floor;// go and start down collection
            else // all upper floor allready served
            {
                //change direction and start again
                // if we're not in motion state
                if((auto_instate(auto_hspd)==1)||(auto_instate(auto_lspd)==1))
                    return -1;
                else
                {
					//if(service_p_org_coll_checked) return -1;
                    direction_up=0;
					//service_p_org_coll_checked = 1;
                    goto strt;
                }    
            }
        }
        
    }
    if(!direction_up)
    {
        if(fw==0)
            nearest_ext_floor=get_nearest_floor(r_dir_both_down);//get mixed (internal and external) down calls    
        else if(fw==1)
            nearest_ext_floor=get_nearest_floor(r_dir_both_down);//get  (internal) down calls case of full weight            
        if(nearest_ext_floor==-1)//down direction is empty
        {
                //change direction and start again
                // if we're not in motion state
            if((auto_instate(auto_hspd)==1)||(auto_instate(auto_lspd)==1))
                return -1;
			//if(service_p_org_coll_checked) return -1;
            direction_up=1;
			//service_p_org_coll_checked = 1;
            goto strt1;
        }
        else
            return nearest_ext_floor;
    }   
}

int up_coll()
{
    int nearest_int_floor,nearest_ext_floor,highest_ext_floor;
strt1:
    if(fw==0)
    {
        if((ireq|ereq)==0)//if there no request left exit this function
            return -1;
    }
    else if(fw==1)
    {
        if(ireq==0)//if there no internal request left (case of full weight) exit this function
            return -1;
    }
    nearest_int_floor=get_nearest_floor(r_internal);
    if(fw==0)
    {
        nearest_ext_floor=get_nearest_floor(r_external);
        highest_ext_floor=get_lowest_floor(r_dir_both);
    }
    else if(fw==1)// if the car in full weight don't respond to external hall calls
    {
        nearest_ext_floor=-1;
        highest_ext_floor=get_lowest_floor(r_internal);
    }
strt:
    if(!direction_up)
    {
        //take only internal car calls
        if(nearest_int_floor!=-1)//there is internal request to serve
        {
            return nearest_int_floor; 
        }
        else // there is no internal request to serve
        {
            // check for lowest external in down direction
            if(nearest_ext_floor!=-1)//there is external floor in that direction
                return highest_ext_floor;// go and start down collection
            else // all upper floor allready served
            {
                //change direction and start again
                // if we're not in motion state
                if((auto_instate(auto_hspd)==1)||(auto_instate(auto_lspd)==1))
                    return -1;
                else
                {
                    direction_up=1;
                    goto strt;
                }    
            }
        }
        
    }
    if(direction_up)
    {
        if(fw==0)
            nearest_ext_floor=get_nearest_floor(r_dir_both);//get mixed (internal and external) down calls    
        else if(fw==1)
            nearest_ext_floor=get_nearest_floor(r_internal);//get  (internal) down calls case of full weight            
        if(nearest_ext_floor==-1)//up direction is empty
        {
            //change direction and start again
            // if we're not in motion state
                if((auto_instate(auto_hspd)==1)||(auto_instate(auto_lspd)==1))
                    return -1;
                direction_up=0;
                goto strt1;

        }
        else
            return nearest_ext_floor;
    }   
}

int all_coll()
{
    int nearest_both_floor;
	
    if(fw==0)
    {
        if((ireq|ereq|eereq)==0)//if there no request left exit this function
            return -1;
    }
    else if(fw==1)
    {
        if(ireq==0)//if there no internal request left (case of full weight) exit this function
            return -1;
    }
    
    if(direction_up)
    {
		nearest_both_floor = get_nearest_floor(r_dir_both); //get mixed internal and external up calls
		if(nearest_both_floor == -1 ) nearest_both_floor = get_highest_floor(r_external);
		if ( (nearest_both_floor != -1) && (nearest_both_floor < get_current_floor()) )
		{
			nearest_both_floor = -1;
		}
		 
		if(fw==1)// if the car in full weight don't respond to external hall calls
		{
			nearest_both_floor = get_nearest_floor(r_internal);
		}
        //take all car calls
        if(nearest_both_floor != -1)//there is request to serve
        {
            return nearest_both_floor; 
        }
        else // there is no request to serve
        {

            // all upper floors already served
            
                //change direction and start again
                // if we're not in motion state
                if((auto_instate(auto_hspd)==1)||(auto_instate(auto_lspd)==1)||(auto_instate(auto_stopping)==1))
                    return -1;
                else
                {
                    direction_up=0;
                    //goto strt;
                }    
            
        }
        
    }
    if(!direction_up)
    {
        
		nearest_both_floor=get_nearest_floor(r_dir_both_down);//get mixed (internal and external) down calls    
        if(nearest_both_floor == -1 ) nearest_both_floor = get_lowest_floor(r_eexternal);
		if ( (nearest_both_floor != -1) && (nearest_both_floor > get_current_floor()) )
		{
			nearest_both_floor = -1;
		}
		
		if(fw==1)
            nearest_both_floor=get_nearest_floor(r_internal);//get  (internal) down calls case of full weight             
        if(nearest_both_floor==-1)//down direction is empty
        {
            if((auto_instate(auto_hspd)==1)||(auto_instate(auto_lspd)==1)||(auto_instate(auto_stopping)==1))
                return -1;
            
            direction_up=1;
            //goto strt1;
        }
        else
            return nearest_both_floor;
    }   
}


int get_nearest_floor(char type)
{
    char current_f,c_start;
    int i;
    unsigned int search_pattern;//to search in
    current_f=get_current_floor();
    switch(type)
    {
        case r_internal:
            search_pattern=ireq;
        break;
        case r_external:
            search_pattern=ereq;
        break;
        case r_dir_both:
            search_pattern=ireq|eereq;
        break;    
		case r_dir_both_down:
		search_pattern = ireq|ereq;
		break;
	}
        
    if(!direction_up)
    {
        c_start=current_f;
        /*
        if(infloorzone()==0)
            ++c_start;    */
        for(i=c_start; i>=0; i--)
        {
            if((search_pattern & msk(i))!=0)
                return i;
        }
        return -1;
    }
    else
    {
        for(i=current_f; i<=f_max; i++)//till maximum floor search
        {
            if((search_pattern & msk(i))!=0)
                return i;
        }
        return -1;    
    }

}

int get_highest_floor(char type)
{
    int i;
    uint_t search_pattern;//to search in
    switch(type)
    {
        case r_internal:
            search_pattern=ireq;
        break;
        case r_external:
            search_pattern=ereq;
        break;
        case r_eexternal:
			search_pattern=eereq;
        break;
		case r_dir_both:
            search_pattern=ireq|ereq;
        break;    
    }
        

        for(i=f_max;i>=0;i--)
        {
            if((search_pattern & msk(i))!=0)
                return i;
        }
        return -1;

}

int get_lowest_floor(char type)
{
    int i;
    unsigned int search_pattern;//to search in
    switch(type)
    {
        case r_internal:
            search_pattern=ireq;
        break;
        case r_external:
            search_pattern=ereq;
        break;
        case r_eexternal:
			search_pattern=eereq;
        break;
		case r_dir_both:
            search_pattern=ireq|ereq;
        break;    
    }
        

        for(i=0;i<=f_max;i++)
        {
            if((search_pattern & msk(i))!=0)
                return i;
        }
        return -1;

}
char check_service()
{
    if((ereq|ireq|eereq)!=0)
        return 1;
    else
        return 0;
}