#pragma once

#include <string>;

const std::string testWorld = // static rigid connection
R"(
4 32 72

S-35 S0
S-25 S0
S20 S0
S30 S0
R-30 R5
R-35 R10
R-25 R10
R-30 R15
R-35 R20
R-25 R20
R-25 R30
R25 R5
R20 R10
R30 R10
R25 R15
R30 R20
R20 R20
R20 R30
R5 R10
R-10 R10
R-5 R10
R0 R10
R0 R5
R-5 R5
R0 R15
R-5 R20
R0 R25
R-5 R25
R0 R20
R-5 R15
R-10 R0
R5 R0
R0 R30
R-5 R35
R0 R35
R-5 R30

s0 s1
s1 r0
r0 r1
r1 s0
s0 r0
r0 r2
r2 s1
r1 r2
r2 r3
r3 r4
r4 r1
r1 r3
r3 r5
r5 r2
r5 r4
r4 r6
r6 r5
s2 s3
s3 r7
r7 r8
r8 s2
s2 r7
r7 r9
r9 s3
r9 r8
r8 r10
r10 r11
r11 r9
r9 r10
r10 r12
r12 r8
r12 r11
r11 r13
r13 r12
r13 r14
r15 r6
r15 r16
r16 r17
r17 r14
r14 r18
r18 r17
r17 r19
r19 r15
r16 r19
r19 r18
r16 r20
r20 r21
r21 r22
r22 r23
r23 r24
r24 r25
r25 r17
r17 r20
r20 r24
r24 r22
r23 r21
r21 r25
r25 r16
r15 r26
r26 r19
r18 r27
r27 r14
r27 r26
r23 r28
r28 r29
r29 r30
r30 r31
r31 r22
r22 r28
r28 r30
r29 r31
r31 r23
)"
;

const std::string fallingStickWorld = // static rigid connection
R"(
0 6 7

R-10 R10
R10 R10
R-20 R10
R-25 R10
R-25 R15
R-20 R15

r0 r1
r2 r3
r3 r4
r4 r5
r5 r2
r2 r4
r3 r5
)"
;

const std::string bridgeWorld = // static rigid connection
R"(
2 3 7

S-20 S0
S20 S0

R0 R0
R-10 R20
R10 R20

s0 r0
s0 r1
s1 r0
s1 r2

r0 r1
r0 r2
r1 r2

)"
;

const std::string bridgeLongWorld = // static rigid connection
R"(
2 7 15

S-20 S0
S20 S0

R-15 R-10
R-5 R-10
R5 R-10
R15 R-10

R-10 R0
R0 R0
R10 R0

s0 r0
s0 r4
s1 r3
s1 r6

r0 r1
r1 r2
r2 r3

r4 r5
r5 r6

r0 r4
r4 r1
r1 r5
r5 r2
r2 r6
r6 r3
)"
;

const std::string bridgeHugeWorld = // static rigid connection
R"(
2 13 14

S-35 S10
S35 S10
R-30 R10
R-25 R10
R-20 R10
R-15 R10
R-10 R10
R-5 R10
R0 R10
R5 R10
R10 R10
R15 R10
R20 R10
R25 R10
R30 R10

s0 r0
r0 r1
r1 r2
r2 r3
r3 r4
r4 r5
r5 r6
r6 r7
r7 r8
r8 r9
r9 r10
r10 r11
r11 r12
r12 s1
)"
;
