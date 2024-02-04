from PIL import Image
from sys import argv

def splitStillImage(path):
    image = Image.open(path)
    image = image.convert("RGB")
    pxMatrix = image.load()
    print("uint32_t frame[256] = [")
    for x in range(16):
        for y in range(16):
            pixel = pxMatrix[x, y]
            color = (pixel[0] << 16) | (pixel[1] << 8) | pixel[2]
            print(color,end="")
            if (x != 15 or y != 15):
                print(",", end="")
    print("]")


def main():
    if len(argv) >= 2:
        splitStillImage(argv[1])
    else:
        print(">> Usage: py imgToPixels.py <path_to_your_image>")
        


if __name__ == "__main__":
    main()