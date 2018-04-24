#ifndef __JEG_CFG_H__
#define __JEG_CFG_H__

#ifndef ENABLED
#   define              ENABLED         1
#endif
#ifndef DISABLED
#   define              DISABLED        0
#endif

/*----------------------------------------------------------------------------*
 * JEG Optimisation / Configuration Switches                                  *
 *----------------------------------------------------------------------------*/

/*! \brief This switch is used to use a dedicated draw pixel interface (callback)
 *!        to enable further pixel based optimisation, such as dirty-matrix which 
 *!        highly relys on the tracking of individual pixel drawing behaviour.
 */
#ifndef JEG_USE_EXTERNAL_DRAW_PIXEL_INTERFACE
#define JEG_USE_EXTERNAL_DRAW_PIXEL_INTERFACE       DISABLED
#endif

/*! \brief This switch is used to fetch the source address of the target memory 
 *!        and use memcpy to replace the byte-by-byte bus_read access. 
 */
#ifndef JEG_USE_DMA_MEMORY_COPY_ACCELERATION
#define JEG_USE_DMA_MEMORY_COPY_ACCELERATION        DISABLED
#endif

/*! \brief This switch is used to add a didicated 16bit bus read/write interface
 *!        to accelerate a little bit
 */
#ifndef JEG_USE_EXTRA_16BIT_BUS_ACCESS
#define JEG_USE_EXTRA_16BIT_BUS_ACCESS              DISABLED
#endif

/*! \brief This switch is used to enable the dummy read existing in real hardware.
 *!        As it is rarely required by games (and actually it is a hardware bug),
 *!        the switch is disabled by default to improve performance
 */
#ifndef JEG_USE_DUMMY_READS
#define JEG_USE_DUMMY_READS                         DISABLED
#endif

/*! \brief This switch is used to enable the 6502 decimal mode which is not 
 *!        supported in NES
 */
#ifndef JEG_USE_6502_DECIMAL_MODE
#define JEG_USE_6502_DECIMAL_MODE                   DISABLED
#endif

#endif