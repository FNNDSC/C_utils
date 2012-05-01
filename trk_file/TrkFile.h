//
//
//  Description:
//      Definition of class that loads a TRK file from the Diffusion Toolkit.
//      For doc, see: http://trackvis.org/docs/?subsect=fileformat
//
//  Author:
//      Dan Ginsburg
//
//  Children's Hospital Boston
//  GPL v2
//
#ifndef TRK_FILE_H
#define TRK_FILE_H

#pragma pack(push)
#pragma pack(1)
// The TrkFileHeader structure comes from the documentation on:
//   http://trackvis.org/docs/?subsect=fileformat
// The structure alignment must be set to 1-byte which is the
// reason for the #pragma pack(1)
typedef struct
{
    char            id_string[6];
    short           dim[3];
    float           voxel_size[3];
    float           origin[3];
    short           n_scalars;
    char            scalar_name[10][20];
    short           n_properties;
    char            property_name[10][20];
    float           vox_to_ras[4][4];
    char            reserved[444];
    char            voxel_order[4];
    char            pad2[4];
    float           image_orientation_patient[6];
    char            pad1[2];
    unsigned char   invert_x;
    unsigned char   invert_y;
    unsigned char   invert_z;
    unsigned char   swap_xy;
    unsigned char   swap_yz;
    unsigned char   swap_zx;
    int             n_count;
    int             version;
    int             hdr_size;
} TrkFileHeader;

// An individual point on a track containing a position
// and list of scalars (of size TrkFileHeader.n_scalars)
typedef struct
{
    float           xyz[3];
    float          *scalars;
} TrackPoint;

// An individual track with "m" points and TrkFileHeader.n_properties
// number of properties
typedef struct
{
    int             m;
    TrackPoint     *points;
    float          *properties;
} Track;
#pragma pack(pop)

///
/// \class TrkFile
/// \brief This class provides method for loading a Diffusion Toolkit dti_recon (.trk) file
///        from disk.
///
class TrkFile
{
public:
    ///
    /// Constructor
    ///
    TrkFile();

    ///
    /// Destructor
    ///
    virtual ~TrkFile();

    ///
    /// Read a '.trk' file into the object
    /// \param filePath Location of 'trk' file to read
    /// \return True if the file was read succesfully, false otherwise.  Errors
    ///         will be printed to stderr
    ///
    bool ReadFile(const char *filePath);

    ///
    /// Get the Track file header
    /// \return Track file header.  File should be loaded before using.
    ///
    const TrkFileHeader& GetHeader() const;

    ///
    /// Get the Tracks
    /// \return Tracks if file was loaded, NULL otherwise.
    ///
    const Track* GetTracks() const;

    ///
    /// Print header
    /// \param outFile Optional parameter specifying file to output, will use
    ///                stdout otherwise
    ///
    const void PrintHeader(FILE *outFile = NULL) const;

protected:

    ///
    /// Clean up memory for tracts (used internally)
    ///
    void Cleanup();

protected:

    /// Track file header
    TrkFileHeader mHeader;

    /// Track file data
    Track *mTracks;
};

#endif
