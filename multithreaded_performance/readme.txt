
NAME: Masataka Mori

Result of time

time ./srt 1-test.ppm >1-test.ppm.tmp
     real    1m16.317s
     user    1m13.790s
     sys     0m0.006s

time ./srt 2-test.ppm >2-test.ppm.tmp
     real    0m48.560s
     user    1m17.899s
     sys     0m0.018s

time ./srt 4-test.ppm >4-test.ppm.tmp
     real    0m33.696s
     user    1m18.317s
     sys     0m0.013s

time ./srt 8-test.ppm >8-test.ppm.tmp
     real    0m21.978s
     user    1m18.440s
     sys     0m0.006s

The issue I ran into was to create the array for printing.
Because I calculated wrongly for each index to hold all printing value,
I continuely overwritten the same index, and it always gave me all 0s as a result.
By change the array 3 dimention, because if we have px, py, [0-2],
there will be no overlap for sure, and it easily solved my issue.

By focusing on real time, it is clear that the more having
thread, the more improvement we could get.
From one threads to 2 threads, we could decrease apploximately
30 seconds. From, 2 to 4, 4 to 8, we can observe that about 10 seconds decrease
each time.
However, notice that when we focus on user time, it is increasing
as increasing the number of threads. User time is defined as CPU time spend
in user mode, so the time executing our program. So it indicates that
by creating threads, it increase CPU time in user mode. Therefore,
we need to think about the trade off because if we increased the number of
threads so much, it costs more on CPU time, and it might cause slower result than
fewer threads.

