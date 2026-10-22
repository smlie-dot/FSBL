#include "extmem_manager.h"
#include <string.h>

void EXTMEM_MANAGER_Init(void)
{
  memset(extmem_list_config, 0x0, sizeof(extmem_list_config));
  extmem_list_config[0].MemType = EXTMEM_USER;
  EXTMEM_Init(EXTMEMORY_1, 0U);
}