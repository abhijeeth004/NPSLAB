#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUCKET_SIZE 10  // Maximum size of the bucket (in bytes)
#define OUTGOING_RATE 1     // Rate at which packets leave the bucket (in bytes per second)
#define PACKET_SIZE 4       // Size of each incoming packet (in bytes)

void process_packet(int *bucket, int *conforming, int *nonconforming, int time);
void display_status(int bucket, int time, int conforming, int nonconforming);

int main() {
    int time_intervals[] = {1, 2, 3, 5, 6, 8, 11, 12, 15, 16, 19};  // Time intervals when packets arrive
    int total_time_intervals = sizeof(time_intervals) / sizeof(time_intervals[0]);
    
    int bucket = 0;  // Initially, the bucket is empty
    int conforming = 0, nonconforming = 0;  // Counters for conforming and nonconforming packets
    
    // Simulate the packet arrival and processing
    for (int i = 0; i < total_time_intervals; i++) {
        int current_time = time_intervals[i];
        
        // Process the packet arrival at the given time
        process_packet(&bucket, &conforming, &nonconforming, current_time);
        
        // Display the status at each time step
        display_status(bucket, current_time, conforming, nonconforming);
    }
    
    printf("\nFinal Statistics:\n");
    printf("Conforming packets: %d\n", conforming);
    printf("Nonconforming packets: %d\n", nonconforming);
    
    return 0;
}

void process_packet(int *bucket, int *conforming, int *nonconforming, int time) {
    // First, simulate the outgoing rate of 1 byte per second
    if (*bucket > 0) {
        (*bucket)--;
    }

    // Incoming packet processing
    if (*bucket + PACKET_SIZE <= MAX_BUCKET_SIZE) {
        // If the bucket can accommodate the new packet
        *bucket += PACKET_SIZE;
        (*conforming)++;  // It's a conforming packet
    } else {
        // If the bucket overflows
        (*nonconforming)++;  // It's a nonconforming packet
    }
}

void display_status(int bucket, int time, int conforming, int nonconforming) {
    printf("Time: %d\n", time);
    printf("Bucket Status: %d bytes\n", bucket);
    printf("Conforming packets: %d\n", conforming);
    printf("Nonconforming packets: %d\n", nonconforming);
    printf("-------------------------\n");
}
