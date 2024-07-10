#ifndef MONITOR_RO_SECTION
#define MONITOR_RO_SECTION

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * FUNCTION     :   monitor_ro_section
 * DESCRIPTION  :
 *      Monitor any write to RO section with bus monitor. The address range 
 *      monitored is from 0x00000000 to section_size - 1 at most, which depends on
 *      the address mask generated on the section_size.
 * PARAMETERS   :
 *      section_size - The size in byte of the RO section.  
 * RETURNS      :
 *      None.
 ******************************************************************************/     
void monitor_ro_section(unsigned long section_size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MONITOR_RO_SECTION */
