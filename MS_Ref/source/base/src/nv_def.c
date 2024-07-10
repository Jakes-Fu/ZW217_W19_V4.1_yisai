/*
    this file include all default data of nvitem.
*/

#include "sci_types.h"
#include "nv_id.h"
#include "nvitem.h"




/*
    the default data example:
    const nvitem_xxx_type  nvitem_xxx=
    {
        ...........
    };
*/


/*
    When power on, system will copy all default data to flash if it is not exist in flash 
    And you needn't write the default data before read.
*/
const NVITEM_DEFAULT_S NVITEM_DEFAULT_VALUE[] = 
{
    /*
        added your default value here .
        NVITEM_ID_XXX must be defined in nv_id.h.
        
        Eg: 
        { NVITEM_ID_xxx, sizeof( nvitem_xxx), (char *)&nvitem_xxx }.
        the nvitem_xxx is the default data of the nvitem.
    */
    
    { 0, 0 , (char *)0 },  /* don't delete this line */
};
