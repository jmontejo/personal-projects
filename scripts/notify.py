''' Runs in a cronjob the commands specified in notify.txt
    For every command that runs successfully it sends a notification in
    mail and moves the command to notify.txt.old
'''

def main():
    with open("notify.txt") as infile:
        for line in infile:
            status = os.system(line)
    

if __name__ == "__main__":
  parser = OptionParser() 
  parser.add_option ("-v","--verbose", 
                     help="Print files as they are processed",
                     dest="verbose", 
                     action="store_true", 
                     default=False) 
  import sys
  sys.exit( main() )
