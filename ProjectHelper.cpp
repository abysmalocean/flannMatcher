#include "ProjectHelper.h"
#ifndef __OPENCV_NONFREE_FEATURES_2D_HPP__
#define __OPENCV_NONFREE_FEATURES_2D_HPP__

#include "opencv2/features2d/features2d.hpp"

#ifdef __cplusplus

namespace cv
{

/*!
 SIFT implementation.

 The class implements SIFT algorithm by D. Lowe.
*/
class CV_EXPORTS_W SIFT : public Feature2D
{
public:
    CV_WRAP explicit SIFT( int nfeatures=0, int nOctaveLayers=3,
          double contrastThreshold=0.04, double edgeThreshold=10,
          double sigma=1.6);

    //! returns the descriptor size in floats (128)
    CV_WRAP int descriptorSize() const;

    //! returns the descriptor type
    CV_WRAP int descriptorType() const;

    //! finds the keypoints using SIFT algorithm
    void operator()(InputArray img, InputArray mask,
                    vector<KeyPoint>& keypoints) const;
    //! finds the keypoints and computes descriptors for them using SIFT algorithm.
    //! Optionally it can compute descriptors for the user-provided keypoints
    void operator()(InputArray img, InputArray mask,
                    vector<KeyPoint>& keypoints,
                    OutputArray descriptors,
                    bool useProvidedKeypoints=false) const;

    AlgorithmInfo* info() const;

    void buildGaussianPyramid( const Mat& base, vector<Mat>& pyr, int nOctaves ) const;
    void buildDoGPyramid( const vector<Mat>& pyr, vector<Mat>& dogpyr ) const;
    void findScaleSpaceExtrema( const vector<Mat>& gauss_pyr, const vector<Mat>& dog_pyr,
                                vector<KeyPoint>& keypoints ) const;

protected:
    void detectImpl( const Mat& image, vector<KeyPoint>& keypoints, const Mat& mask=Mat() ) const;
    void computeImpl( const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptors ) const;

    CV_PROP_RW int nfeatures;
    CV_PROP_RW int nOctaveLayers;
    CV_PROP_RW double contrastThreshold;
    CV_PROP_RW double edgeThreshold;
    CV_PROP_RW double sigma;
};

typedef SIFT SiftFeatureDetector;
typedef SIFT SiftDescriptorExtractor;

/*!
 SURF implementation.

 The class implements SURF algorithm by H. Bay et al.
 */
class CV_EXPORTS_W SURF : public Feature2D
{
public:
    //! the default constructor
    CV_WRAP SURF();
    //! the full constructor taking all the necessary parameters
    explicit CV_WRAP SURF(double hessianThreshold,
                  int nOctaves=4, int nOctaveLayers=2,
                  bool extended=true, bool upright=false);

    //! returns the descriptor size in float's (64 or 128)
    CV_WRAP int descriptorSize() const;

    //! returns the descriptor type
    CV_WRAP int descriptorType() const;

    //! finds the keypoints using fast hessian detector used in SURF
    void operator()(InputArray img, InputArray mask,
                    CV_OUT vector<KeyPoint>& keypoints) const;
    //! finds the keypoints and computes their descriptors. Optionally it can compute descriptors for the user-provided keypoints
    void operator()(InputArray img, InputArray mask,
                    CV_OUT vector<KeyPoint>& keypoints,
                    OutputArray descriptors,
                    bool useProvidedKeypoints=false) const;

    AlgorithmInfo* info() const;

    CV_PROP_RW double hessianThreshold;
    CV_PROP_RW int nOctaves;
    CV_PROP_RW int nOctaveLayers;
    CV_PROP_RW bool extended;
    CV_PROP_RW bool upright;

protected:

    void detectImpl( const Mat& image, vector<KeyPoint>& keypoints, const Mat& mask=Mat() ) const;
    void computeImpl( const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptors ) const;
};

typedef SURF SurfFeatureDetector;
typedef SURF SurfDescriptorExtractor;

} /* namespace cv */

#endif /* __cplusplus */

#endif

/* End of file. */
