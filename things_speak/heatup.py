#!/usr/bin/env python3

import time

def heat_up():
	start_time = time.time()
	print("Running program to generate heat...")
	while True:
		_ = [x ** 2 for x in range(1_000_000)] 
		if time.time() - start_time >= 10:
			print("breaking")
			time.sleep(0.5)
			break


heat_up()
