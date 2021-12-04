import statistics

uniform_samples = list(map(float, input().split()))
interarrival_samples = list(map(float, input().split()))
service_samples = list(map(float, input().split()))

uniform_min = min(uniform_samples)
interarrival_min = min(interarrival_samples)
service_min = min(service_samples)

uniform_max = max(uniform_samples)
interarrival_max = max(interarrival_samples)
service_max = max(service_samples)

uniform_median = statistics.median(uniform_samples)
interarrival_median = statistics.median(interarrival_samples)
service_median = statistics.median(service_samples)

f = open("output_c.txt", "w");

f.write("Unform distribution min = " + str(uniform_min) + "\n")
f.write("Unform distribution max = " + str(uniform_max) + "\n")
f.write("Unform distribution median = " + str(uniform_median) + "\n\n\n")

f.write("Interarrival distribution min = " + str(interarrival_min) + "\n")
f.write("Interarrival distribution max = " + str(interarrival_max) + "\n")
f.write("Interarrival distribution median = " + str(interarrival_median) + "\n\n\n")

f.write("Service distribution min = " + str(service_min) + "\n")
f.write("Service distribution max = " + str(service_max) + "\n")
f.write("Service distribution median = " + str(service_median) + "\n")

f.close()