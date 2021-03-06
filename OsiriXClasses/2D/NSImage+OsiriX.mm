/*=========================================================================
  Program:   OsiriX

  Copyright (c) OsiriX Team
  All rights reserved.
  Distributed under GNU - LGPL
  
  See http://www.osirix-viewer.com/copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.
=========================================================================*/

#import "NSImage+OsiriX.h"
#import "N2Debug.h"

#include "options.h"

#define FIX_ISSUE_9

#ifndef FIX_ISSUE_9
extern unsigned char* compressJPEG(int inQuality, unsigned char* inImageBuffP, int inImageHeight, int inImageWidth, int monochrome, int *destSize);
extern NSRecursiveLock* PapyrusLock;
#endif

@implementation NSImage (OsiriX)

-(NSData*)JPEGRepresentationWithQuality:(CGFloat)quality
{
	NSBitmapImageRep* imageRep = [NSBitmapImageRep imageRepWithData:self.TIFFRepresentation];
	NSData* result = NULL;
	
#ifndef FIX_ISSUE_9
	if ([imageRep bitsPerPixel] == 8)
	{
		[PapyrusLock lock];
		
		@try
		{
			int size;
            // TODO: call a suitable JPEG codec, maybe in the DCMTK library
            unsigned char* p = compressJPEG(quality*100, [imageRep bitmapData], [imageRep pixelsHigh], [imageRep pixelsWide], 1, &size);

			if (p)
				result = [NSData dataWithBytesNoCopy: p length: size freeWhenDone: YES];
		}
		@catch (NSException * e)
		{
            N2LogExceptionWithStackTrace(e);
		}
		
		[PapyrusLock unlock];
	}
	else
#endif
	{
		NSDictionary* imageProps = [NSDictionary dictionaryWithObject:[NSNumber numberWithFloat:quality] forKey:NSImageCompressionFactor];
		result = [imageRep representationUsingType:NSJPEGFileType properties:imageProps];
		//NSJPEGFileType	NSJPEG2000FileType <- MAJOR memory leak with NSJPEG2000FileType when reading !!! Kakadu library...
	}
	
	return result;	
}

@end

