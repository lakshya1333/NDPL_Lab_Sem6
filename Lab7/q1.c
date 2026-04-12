#include <stdio.h>

int main() {
    int arrival_time[] = {1, 2, 3, 5, 6, 8, 11, 12, 15, 16, 19};
    int n = sizeof(arrival_time) / sizeof(arrival_time[0]);

    int bucket_size = 10;   // Max capacity
    int output_rate = 1;    // bytes per second
    int packet_size = 4;    // each packet size

    int current_bucket = 0;
    int last_time = 0;

    printf("Time\tBucketBefore\tPacket\tStatus\t\tBucketAfter\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        int time = arrival_time[i];

        // Leak data between last_time and current arrival time
        int time_diff = time - last_time;
        int leaked = time_diff * output_rate;

        if (leaked > current_bucket)
            current_bucket = 0;
        else
            current_bucket -= leaked;

        printf("%d\t%d\t\t%d\t", time, current_bucket, packet_size);

        // Check if packet can be added
        if (current_bucket + packet_size <= bucket_size) {
            current_bucket += packet_size;
            printf("Conforming\t%d\n", current_bucket);
        } else {
            printf("Non-Conforming\t%d\n", current_bucket);
        }

        last_time = time;
    }

    return 0;
}