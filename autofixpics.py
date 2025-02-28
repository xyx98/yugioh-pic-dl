import os,sys
import argparse
from PIL import Image,ImageOps
from multiprocessing import pool
from functools import partial


def process(i,indir,outdir):
    try:
        img=Image.open(os.path.join(indir,i))
    except:
        print(f"open {i} fail.")
        return

    filename=os.path.splitext(i)[0]
    img=img.convert(mode="RGBA")
    w,h=img.size
    if w<h:
        print(i,w,h,sep="\t")
        img=img.crop([0,0,w,w])
    elif w>h:
        print(i,w,h,sep="\t")
        img=ImageOps.expand(img, (0,0,0,w-h), fill=(255,255,255,0))

    img.save(os.path.join(outdir,filename+".png"),format="png")

def main(indir='pics',outdir='fix',suffix=['jpg','png','webp'],thread=8,skipexist=True):
    if not os.path.exists(outdir):
        os.mkdir(outdir)

    plist=[]
    for i in os.listdir(indir):
        filename,ext=os.path.splitext(i)
        ext=ext[1:].lower()
        if ext not in suffix:
            continue
        if skipexist and os.path.exists(os.path.join(outdir,filename+".png")):
            continue
        
        plist.append(i)
    
    pf=partial(process,indir=indir,outdir=outdir)

    with pool.Pool(thread) as pl:
            pl.map(pf,plist)
        #process(i,indir,outdir)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-i','--input', nargs='?',default='pics',type=str ,help='input dir for downloaded pics,default is pics.')
    parser.add_argument('-o','--output',nargs='?',default='fix' ,type=str ,help='output dir for pics,default is fix')
    parser.add_argument('-t','--thread',nargs='?',default=8     ,type=int ,help='set used thread,default is 8.')
    
    parser.add_argument('-s','--suffix',nargs='?',default="jpg,png,webp",type=str,help='set file extensions to process.')
    parser.add_argument('-ns','--no-skip',action="store_true",default=False,help='do not skip processed file.')
    args=parser.parse_args(sys.argv[1:])
    print(args)
    main(indir=args.input,
         outdir=args.output,
         thread=args.thread,
         suffix=args.suffix.split(','),
         skipexist=not args.no_skip
         )