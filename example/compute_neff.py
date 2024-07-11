import sys
import neffy

def main():
    # Get arguments from the command line
    args = sys.argv[1:]
    
    if not args:
        print("Please provide arguments for the neff tool.")
        return
    
    try:
        output = neffy.compute_neff(args)
        print(output)
    except RuntimeError as e:
        print(f"{e}")

if __name__ == "__main__":
    main()
