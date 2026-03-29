#pragma once

#include "diagnostics.h"
#include <arduino.h>
#include "esp_ota_ops.h"

void printPartitionInfo() {
  // Get metadata for the partition currently running this code
  const esp_partition_t* running = esp_ota_get_running_partition();

  Serial.println("--- Active Partition Info ---");
  Serial.printf("+ Label:   %s\n", running->label);
  Serial.printf("+ Address: 0x%08X\n", running->address);
  Serial.printf("+ Size:    %u bytes (%.2f MB)\n", running->size, running->size / 1048576.0);
  Serial.println("-----------------------------");
}