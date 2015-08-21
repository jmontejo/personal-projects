
# ---------------
def getTimeStamp(hour=False):
  """
  Returns timestamp as a string in the format yyyy_mm_dd
  """
  import time
  if hour:
    now = time.localtime()
    now_str = "%d_%02d_%02d_%02d_%02d_%02d" % (now.tm_year, now.tm_mon, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec)
    return now_str
  else:
    today = time.localtime()
    today_str = "%d_%02d_%02d" % (today.tm_year, today.tm_mon, today.tm_mday)
    return today_str
