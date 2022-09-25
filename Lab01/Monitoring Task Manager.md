Dean Shin COMP 173 Fall 2022


1. 
```
deanshinpersonal@dhcp-10-15-165-16 ~ % date

Wed Aug 31 18:43:59 PDT 2022

deanshinpersonal@dhcp-10-15-165-16 ~ % time

shell  0.01s user 0.01s system 0% cpu 2:53.61 total

children  0.00s user 0.01s system 0% cpu 2:53.61 total
```

2. 

N/A

3. 
   
% CPU, % GPU are the columns that showcase CPU and GPU performance respectively
![[Screen Shot 2022-09-03 at 2.36.18 PM.png]]

4. 
N/A
5. 

Google Chrome:
%CPU: 2.3, %GPU: 0.0
Google Chrome Helper (GPU):
%CPU: 2.6, %GPU: 10.6
Google Chrome Helper (Renderer):
%CPU: 1.7, %GPU: 0.0

![[Screen Shot 2022-09-03 at 2.40.37 PM.png]]

6. 

Google Chrome Helper (Renderer):
%CPU: 2.8, %GPU: 0.0
Google Chrome Helper (GPU):
%CPU: 2.2, %GPU: 0.7
Google Chrome Helper:
%CPU: 0.2, %GPU: 0.0
Google Chrome:
%CPU: 0.1, %GPU: 0.0

![[Screen Shot 2022-09-03 at 2.42.56 PM.png]]
   
7. 

The Activity Monitor process is utilizing the most CPU

8. 

Yes, I am able to change the priority of a process by utilizing the `sudo renice` command in the terminal, however, changing the priority between maximum priority (-20) and minimum priority (20) doesn't seem to be having any effect that I can discern. The CPU of Google Chrome remains around 0.1%, and only changes when the Google Chrome window is focused on.
![[Screen Shot 2022-09-03 at 2.48.51 PM.png]]

9. 

Through Activity Monitor, I can Stop (either Quit or Force Quit) a process. I can also view individual statistics (e.g. Threads, CPU Time, Unix System Calls, Open Files & Ports) about a process by pressing the info button on the activity monitor. I can also take a Sample of a Process, which provides a snapshot of what a process is doing at a particular moment, which is extremely useful when debugging processes.

10. 

Ending the process removes it from the activity monitor, since the process is no longer running. Doing so on my computer had no hugely discernable impact on the computers' CPU Load, but that may be because Chrome was not performing a particularly strenuous task.


 ![[Screen Shot 2022-09-03 at 2.57.27 PM.png]]
11. 
N/A