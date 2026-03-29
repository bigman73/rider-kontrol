#pragma once

/**
 * @brief Retrieves and logs metadata about the currently executing flash partition.
 *
 * Useful for verifying OTA updates on the Lolin C3 Mini: identifies whether the
 * sketch runs from the primary (app0) or secondary (app1) slot from partitions.csv.
 */
 void printPartitionInfo();