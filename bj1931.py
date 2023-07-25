import sys

n = int(sys.stdin.readline().strip())
meeting = [tuple(map(int, sys.stdin.readline().strip().split())) for _ in range(n)]
meeting.sort(key=lambda x: (x[1], x[0]))
end_of_meeting = 0
cnt = 0

for i in meeting:
    if i[0] >= end_of_meeting:
        end_of_meeting = i[1]
        cnt += 1

print(cnt)
