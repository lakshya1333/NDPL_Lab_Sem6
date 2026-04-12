#include <stdio.h>

int main() {
    float time = 0.5;
    float end_time = 5.0;   // simulate till 5 sec

    float bucket_size = 50;   // KB
    float tokens = 50;        // initially full
    float rate = 10;          // KB per sec
    float packet_size = 15;   // KB

    printf("Time\tTokens Before\tPacket\tStatus\t\tTokens After\n");
    printf("--------------------------------------------------------------\n");

    while (time <= end_time) {

        // Add tokens (refill every 0.5 sec → rate * 0.5)
        tokens += rate * 0.5;

        if (tokens > bucket_size)
            tokens = bucket_size;

        printf("%.1f\t%.2f\t\t%.2f\t", time, tokens, packet_size);

        if (tokens >= packet_size) {
            tokens -= packet_size;
            printf("Sent\t\t%.2f\n", tokens);
        } else {
            printf("Queued\t\t%.2f\n", tokens);
        }

        time += 0.5;
    }

    return 0;
}