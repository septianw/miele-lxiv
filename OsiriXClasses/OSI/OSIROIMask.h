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

#import <Cocoa/Cocoa.h>
#import "N3Geometry.h"
#import "OSIFloatVolumeData.h"

@class OSIFloatVolumeData;

/** A structure used to describe a single run length of a mask.
 
 */

struct OSIROIMaskRun {
	NSRange widthRange;
    NSUInteger heightIndex;
    NSUInteger depthIndex;
    float intensity;
};
typedef struct OSIROIMaskRun OSIROIMaskRun;

extern const OSIROIMaskRun OSIROIMaskRunZero;

/** Returns a newly created OSIROIMaskRun
 
 */
OSIROIMaskRun OSIROIMaskRunMake(NSRange widthRange, NSUInteger heightIndex, NSUInteger depthIndex, float intensity);

/** Returns the first width index that is under the run.

 */
NSUInteger OSIROIMaskRunFirstWidthIndex(OSIROIMaskRun maskRun);

/** Returns the last width index that is under the run.

 */
NSUInteger OSIROIMaskRunLastWidthIndex(OSIROIMaskRun maskRun);

/** A structure used to describe a single point in a mask.
 
 */

struct OSIROIMaskIndex {
	NSUInteger x;
	NSUInteger y;
	NSUInteger z;
};
typedef struct OSIROIMaskIndex OSIROIMaskIndex;

CF_EXTERN_C_BEGIN

/** Transforms the maskIndex using the given transform, and returns an N3Vector
 
 */
N3Vector OSIROIMaskIndexApplyTransform(OSIROIMaskIndex maskIndex, N3AffineTransform transform);

/** Returns YES if the `maskIndex` is withing the `maskRun`
 
 */
BOOL OSIROIMaskIndexInRun(OSIROIMaskIndex maskIndex, OSIROIMaskRun maskRun);
/** Returns an array of all the OSIROIMaskIndex structs in the `maskRun`.
 
 */
NSArray *OSIROIMaskIndexesInRun(OSIROIMaskRun maskRun); // should this be a function, or a class method on OSIROIMask?

/** Returns NSOrderedAscending if `maskRun2` is larger than `maskRun1`.
 
 */
NSInteger OSIROIMaskCompareRunValues(NSValue *maskRun1, NSValue *maskRun2, void *context);

/** Returns a value larger than 0 if `maskRun2` is larger than `maskRun1`.
 
 */
NSComparisonResult OSIROIMaskCompareRun(OSIROIMaskRun maskRun1, OSIROIMaskRun maskRun2);

/** Returns YES if the two mask runs overlap, NO otherwise.
 
 */
BOOL OSIROIMaskRunsOverlap(OSIROIMaskRun maskRun1, OSIROIMaskRun maskRun2);

/** Returns YES if the two mask runs abut, NO otherwise.
 
 */
BOOL OSIROIMaskRunsAbut(OSIROIMaskRun maskRun1, OSIROIMaskRun maskRun2);


CF_EXTERN_C_END

// masks are stored in width direction run lengths

/** `OSIROIMask` instances represent a mask that can be applied to a volume. The Mask itself is stored as a set of individual mask runs.
 
 Stored masks use the following structs.
 
 `test`
 
 
 
 `struct OSIROIMaskRun {`
 
 `NSRange widthRange;`
 
 `NSUInteger heightIndex;`
 
 `float intensity;`
 
 `};`
 
 `typedef struct OSIROIMaskRun OSIROIMaskRun;`
 
 `struct OSIROIMaskIndex {`
 
 `NSUInteger x;`
 
 `NSUInteger y;`
 
 `NSUInteger z;`
 
 `};`
 
 `typedef struct OSIROIMaskIndex OSIROIMaskIndex;`
 
 Use the following functions are also available
 
 `BOOL OSIROIMaskIndexInRun(OSIROIMaskIndex maskIndex, OSIROIMaskRun maskRun);`
 
 `NSArray *OSIROIMaskIndexesInRun(OSIROIMaskRun maskRun);`
 
 
 
 
 */


@interface OSIROIMask : NSObject <NSCopying, NSSecureCoding> {
@private
    NSData *_maskRunsData;
	NSArray *_maskRuns;
}

///-----------------------------------
/// @name Creating ROI Masks
///-----------------------------------

/** Returns a newly created empty ROI Mask.
 
 @return The newly crated and initialized ROI Mask object.
 */
+ (instancetype)ROIMask;

/** Returns a newly created ROI Mask that has the shape of a sphere with the specified diameter.
 
 @return The newly crated and initialized ROI Mask object.
 */
+ (instancetype)ROIMaskWithSphereDiameter:(NSUInteger)diameter;

/** Returns a newly created ROI Mask that has the shape of a cube with the specified size.
 
 @return The newly crated and initialized ROI Mask object.
 */
+ (instancetype)ROIMaskWithCubeSize:(NSUInteger)size;

/** Returns a newly created ROI Mask that has the shape of a box with the specified sizes.

 @return The newly crated and initialized ROI Mask object.
 */
+ (instancetype)ROIMaskWithBoxWidth:(NSUInteger)width height:(NSUInteger)height depth:(NSUInteger)depth;

/** Returns a newly created ROI Mask that has the shape of an elipsoid with the specified sizes.

 @return The newly crated and initialized ROI Mask object.
 */
+ (instancetype)ROIMaskWithElipsoidWidth:(NSUInteger)width height:(NSUInteger)height depth:(NSUInteger)depth;

/** Returns a newly created ROI Mask based on the intesities of the floatVolumeData.
 
 The returned mask  is a mask on the floatVolumeData with the intensities of the floatVolumeData.
 
 @return The newly crated and initialized ROI Mask object or `nil` if there was a problem initializing the object.
 @param floatVolumeData The OSIFloatVolumeData on which to build and base the mask.
 @param volumeTransformPtr Returns the transform needed to go from DICOM space to the mask
 */
+ (instancetype)ROIMaskFromVolumeData:(OSIFloatVolumeData *)floatVolumeData volumeTransform:(N3AffineTransformPointer)volumeTransformPtr;
+ (instancetype)ROIMaskFromVolumeData:(OSIFloatVolumeData *)floatVolumeData __deprecated;

/** Initializes and returns a newly created empty ROI Mask.
 
 Creates an empty ROI Mask.
 
 @return The initialized ROI Mask object.
 */
- (instancetype)init;

// create the thing, maybe we should really be working with C arrays.... or at least give the option
/** Initializes and returns a newly created ROI Mask.
 
 Creates a ROI Mask based on the given individual runs.
 
 @return The initialized ROI Mask object or `nil` if there was a problem initializing the object.
 @param maskRuns An array of OSIROIMaskRun structs in NSValues.
 */
- (instancetype)initWithMaskRuns:(NSArray *)maskRuns;

/** Initializes and returns a newly created ROI Mask.
 
 Creates a ROI Mask based on the given individual runs.
 
 @return The initialized ROI Mask object or `nil` if there was a problem initializing the object.
 @param maskRunData is the serialized OSIROIMaskRuns.
 */
- (instancetype)initWithMaskRunData:(NSData *)maskRunData;

/** Initializes and returns a newly created ROI Mask.
 
 Creates a ROI Mask based on the given individual runs. The mask runs must be sorted.
 
 @return The initialized ROI Mask object or `nil` if there was a problem initializing the object.
 @param maskRunData is the serialized OSIROIMaskRuns.
 */
- (instancetype)initWithSortedMaskRunData:(NSData *)maskRunData;

// create the thing, maybe we should really be working with C arrays.... or at least give the option
/** Initializes and returns a newly created ROI Mask.
 
 Creates a ROI Mask based on the given individual runs. The mask runs must be sorted.
 
 @return The initialized ROI Mask object or `nil` if there was a problem initializing the object.
 @param maskRuns An array of OSIROIMaskRun structs in NSValues.
 */
- (instancetype)initWithSortedMaskRuns:(NSArray *)maskRuns;

/** Initializes and returns a newly created ROI Mask.
 
 Creates a ROI Mask based on the given individual indexes.
 
 @return The initialized ROI Mask object or `nil` if there was a problem initializing the object.
 @param maskRuns An array of OSIROIMaskIndex structs in NSValues.
 */
- (instancetype)initWithIndexes:(NSArray *)maskIndexes;

/** Initializes and returns a newly created ROI Mask.
 
 Creates a ROI Mask based on the given individual indexes.
 
 @return The initialized ROI Mask object or `nil` if there was a problem initializing the object.
 @param indexData is the serialized OSIROIMaskIndexes.
 */
- (instancetype)initWithIndexData:(NSData *)indexData;

/** Initializes and returns a newly created ROI Mask.
 
 Creates a ROI Mask based on the given individual indexes. The mask indexes must be sorted.
 
 @return The initialized ROI Mask object or `nil` if there was a problem initializing the object.
 @param maskRuns An array of OSIROIMaskIndex structs in NSValues.
 */
- (instancetype)initWithSortedIndexes:(NSArray *)maskIndexes;

/** Initializes and returns a newly created ROI Mask.
 
 Creates a ROI Mask based on the given individual indexes. The mask indexes must be sorted.
 
 @return The initialized ROI Mask object or `nil` if there was a problem initializing the object.
 @param maskRuns An array of OSIROIMaskIndex structs in NSValues.
 */
- (instancetype)initWithSortedIndexData:(NSData *)indexData;

///-----------------------------------
/// @name Working with the Mask
///-----------------------------------

/** Returns a mask made by translating the receiever by the given distances.
 
 */
- (OSIROIMask *)ROIMaskByTranslatingByX:(NSInteger)x Y:(NSInteger)y Z:(NSInteger)z;

/** Returns a mask that represents the intersection of the receiver and the given mask .
 
 */
- (OSIROIMask *)ROIMaskByIntersectingWithMask:(OSIROIMask *)otherMask;

/** Returns a mask that represents the union of the receiver and the given mask .
 
 */
- (OSIROIMask *)ROIMaskByUnioningWithMask:(OSIROIMask *)otherMask;

/** Returns a mask formed by subtracting otherMask from the receiver.

 */
- (OSIROIMask *)ROIMaskBySubtractingMask:(OSIROIMask *)otherMask;

/** Returns a OSIFloatVolumeData filled with the intensities of the mask.

 */
- (OSIFloatVolumeData *)floatVolumeDataRepresentationWithVolumeTransform:(N3AffineTransform)volumeTransform;

/** Returns a mask formed by cropping any indexes that are further out than the bounds specified from the receiver.
 
 */
- (OSIROIMask *)ROIMaskCroppedToWidth:(NSUInteger)width height:(NSUInteger)height depth:(NSUInteger)depth;

/** Returns YES if the two masks intersect.
 
 */
- (BOOL)intersectsMask:(OSIROIMask *)otherMask;

/** Returns YES if the two masks are equal.
 
 */
- (BOOL)isEqualToMask:(OSIROIMask *)otherMask;

/** Evaluates a given predicate against each pixel in the mask and returns a new mask containing the pixels for which the predicate returns true.
 
 The evaluated object used for the predicate responds to:
 -(float)intensity; The value of the pixel
 -(float)ROIMaskIntensity; The value of the intesity stored in the mask
 -(NSUInteger)ROIMaskIndexX;
 -(NSUInteger)ROIMaskIndexY;
 -(NSUInteger)ROIMaskIndexZ;
 
 @return The resulting mask after having applied the predicate to the receiver.
 */
- (OSIROIMask *)filteredROIMaskUsingPredicate:(NSPredicate *)predicate floatVolumeData:(OSIFloatVolumeData *)floatVolumeData;

/** Returns the mask as a set ofOSIROIMaskRun structs in NSValues.
 
 @return The mask as a set of OSIROIMaskRun structs in NSValues.
 */
- (NSArray *)maskRuns;

/** Returns the mask as an NSData that contains a C array of OSIROIMaskRun structs.
 
 @return The mask as an NSData that contains a C array of OSIROIMaskRun structs.
 */
- (NSData *)maskRunsData;

/** Returns the count of mask runs.
 
 @return The count of mask runs.
 */
- (NSUInteger)maskRunCount;

/** Returns the mask as a set OSIROIMaskIndex structs in NSValues.
 
 @return The mask as a set OSIROIMaskIndex structs in NSValues.
 */
- (NSArray *)maskIndexes;

/** Returns the count of mask indexes.
 
 @return The count of mask index. (the number of voxels in the mask)
 */
- (NSUInteger)maskIndexCount;

/** Returns YES if the given index is within the mask.
 
 @return YES if the given index is within the mask.
 
 @param index OSIROIMaskIndex struct to test.
 */
- (BOOL)containsIndex:(OSIROIMaskIndex)index;
- (BOOL)indexInMask:(OSIROIMaskIndex)index __deprecated;

/** Returns a mask that has been resampled from being in the volume as the position fromTransform to a mask that is in the volume at position toTransform.

 */
- (instancetype)ROIMaskByResamplingFromVolumeTransform:(N3AffineTransform)fromTransform toVolumeTransform:(N3AffineTransform)toTransform interpolationMode:(CPRInterpolationMode)interpolationsMode;

/** Returns the extent of the receiver. All values are inclusive.

 */
- (void)extentMinWidth:(NSUInteger*)minWidthPtr maxWidth:(NSUInteger*)maxWidthPtr minHeight:(NSUInteger*)minHeightPtr maxHeight:(NSUInteger*)maxHeightPtr minDepth:(NSUInteger*)minDepthPtr maxDepth:(NSUInteger*)maxDepthPtr;

/** Returns an array of points that represent the outside bounds of the mask.
 
 @return An array of N3Vectors stored at NSValues that represent the outside bounds of the mask.
 */
- (NSArray *)convexHull; // N3Vectors stored in NSValue objects. The mask is inside of these points

/** Returns the center of mass of the mask.
 
 @return The center of mass of the mask.
 */
- (N3Vector)centerOfMass;


@end

/** NSValue methods to handle Mask types.
 
 */


@interface NSValue (OSIROIMaskRun)

///-----------------------------------
/// @name OSIROIMaskRun methods
///-----------------------------------

/** Creates and returns an NSValue object that contains a given OSIROIMaskRun structure.
 
 @return A new NSValue object that contains the value of volumeRun.
 @param volumeRun The value for the new object.
 */
+ (NSValue *)valueWithOSIROIMaskRun:(OSIROIMaskRun)volumeRun;

/** Returns an OSIROIMaskRun structure representation of the receiver.
 
 @return An OSIROIMaskRun structure representation of the receiver.
 */
- (OSIROIMaskRun)OSIROIMaskRunValue;

/** Creates and returns an NSValue object that contains a given OSIROIMaskIndex structure.
 
 @return A new NSValue object that contains the value of maskIndex.
 @param maskIndex The value for the new object.
 */
+ (NSValue *)valueWithOSIROIMaskIndex:(OSIROIMaskIndex)maskIndex;

/** Returns an OSIROIMaskIndex structure representation of the receiver.
 
 @return An OSIROIMaskIndex structure representation of the receiver.
 */
- (OSIROIMaskIndex)OSIROIMaskIndexValue;
@end


