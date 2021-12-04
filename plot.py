import statistics

uniform_samples = list(map(float, input().split()))
interarrival_samples = list(map(float, input().split()))
service_samples = list(map(float, input().split()))

print(statistics.mean(uniform_samples))
print(statistics.mean(interarrival_samples))
print(statistics.mean(service_samples))