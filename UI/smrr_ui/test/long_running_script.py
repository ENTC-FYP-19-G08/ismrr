import time

print("hello world", flush=True)
# Simulate a long-running task
for i in range(10):
    print(f"Output {i}", flush=True)
    time.sleep(1)

print("hello", flush=True)
