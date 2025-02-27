import os
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
    main()