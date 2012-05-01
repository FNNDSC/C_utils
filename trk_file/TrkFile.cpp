//
//
//  Description:
//      Implementation of class that loads a TRK file from the Diffusion Toolkit.
//      For doc, see: http://trackvis.org/docs/?subsect=fileformat
//
//  Author:
//      Dan Ginsburg
//
//  Children's Hospital Boston
//  GPL v2
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TrkFile.h"

const int TRK_FILE_VERSION = 2;

///////////////////////////////////////////////////////////////////////////////
//
//  Constructor/Destructor
//
//

///
// Constructor
//
TrkFile::TrkFile():
    mTracks(NULL)
{
    memset(&mHeader, 0, sizeof(mHeader));
}

///
// Destructor
//
TrkFile::~TrkFile()
{
    Cleanup();
}


///
//  Clean up memory for tracts
//
void TrkFile::Cleanup()
{
    if (mTracks != NULL)
    {
        for (int i = 0; i < mHeader.n_count; i++)
        {
            Track *track = &mTracks[i];

            if (track->points != NULL)
            {
                if (mHeader.n_scalars > 0)
                {
                    for (int p = 0; p < track->m; p++)
                    {
                        delete [] track->points[p].scalars;
                    }
                }

                delete [] track->points;

                if (mHeader.n_properties > 0)
                {
                    delete [] track->properties;
                }
            }
        }

        delete [] mTracks;
        mTracks = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
//  Public Methods
//
//

///
//  Read a '.trk' file into the object
//
bool TrkFile::ReadFile(const char *filePath)
{
    Cleanup();

    FILE *fp = fopen(filePath, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening file for reading: '%s'\n", filePath);
        return false;
    }

    size_t bytesRead = fread(&mHeader, 1, sizeof(mHeader), fp);
    if (bytesRead != sizeof(mHeader))
    {
        fprintf(stderr, "Error reading trk file header: '%s'\n", filePath);
        return false;
    }

    if (mHeader.version != TRK_FILE_VERSION)
    {
        fprintf(stderr, "Version mismatch, expected (%d), got (%d)\n", TRK_FILE_VERSION, mHeader.version);
        return false;
    }

    mTracks = new Track[mHeader.n_count];
    for (int i = 0; i < mHeader.n_count; i++)
    {
        fread(&mTracks[i].m, 1, sizeof(int), fp);
        mTracks[i].points = new TrackPoint[mTracks[i].m];
        for (int j = 0; j < mTracks[i].m; j++)
        {
            fread(&mTracks[i].points[j].xyz[0], 3, sizeof(float), fp);

            if (mHeader.n_scalars > 0)
            {
                mTracks[i].points[j].scalars = new float[mHeader.n_scalars];
                fread(mTracks[i].points[j].scalars, mHeader.n_scalars, sizeof(float), fp);
            }
        }

        if (mHeader.n_properties > 0)
        {
            mTracks[i].properties = new float[mHeader.n_properties];
            fread(mTracks[i].properties, mHeader.n_properties, sizeof(float), fp);
        }
    }

    fclose(fp);
    return true;
}

///
//  Get the Track file header
//
const TrkFileHeader& TrkFile::GetHeader() const
{
    return mHeader;
}

///
//  Get the Track file header
//
const Track* TrkFile::GetTracks() const
{
    return mTracks;
}

///
//  Print header
//
const void TrkFile::PrintHeader(FILE *outFile) const
{
    if (outFile == NULL)
    {
        outFile = stdout;
    }

    fprintf(outFile, "id_string:\t\t\t\t%s\n", mHeader.id_string);
    fprintf(outFile, "dim:      \t\t\t\t[%d, %d, %d]\n", mHeader.dim[0], mHeader.dim[1], mHeader.dim[2]);
    fprintf(outFile, "voxel_size:\t\t\t\t[%f, %f, %f]\n", mHeader.voxel_size[0], mHeader.voxel_size[1], mHeader.voxel_size[2]);
    fprintf(outFile, "origin:\t\t\t\t\t[%f, %f, %f]\n", mHeader.origin[0], mHeader.origin[1], mHeader.origin[2]);
    fprintf(outFile, "n_scalars:\t\t\t\t%d\n", mHeader.n_scalars);
    for (int i = 0; i < mHeader.n_scalars; i++)
    {
        fprintf(outFile, "scalar_name:\t\t\t\t%s\n", mHeader.scalar_name[i]);
    }
    fprintf(outFile, "n_properties:\t\t\t\t%d\n", mHeader.n_properties);
    for (int i = 0; i < mHeader.n_properties; i++)
    {
        fprintf(outFile, "property_name:\t\t\t\t%s\n", mHeader.property_name[i]);
    }
    fprintf(outFile, "vox_to_ras:\t\t\t\t[");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            fprintf(outFile, "%f", mHeader.vox_to_ras[i][j]);
            if (!(i == 3 && j == 3))
            {
                fprintf(outFile, ",\t");
            }
        }
        if (i != 3)
        {
            fprintf(outFile, "\n\t\t\t\t\t ");
        }
        else
        {
            fprintf(outFile, "]\n");
        }
    }
    fprintf(outFile, "voxel_order:\t\t\t\t[%c, %c, %c, %c]\n", mHeader.voxel_order[0], mHeader.voxel_order[1],
                                                               mHeader.voxel_order[2], mHeader.voxel_order[3]);

    fprintf(outFile, "image_orientation_patient:\t\t[%f, %f, %f, %f, %f, %f]\n",
                                            mHeader.image_orientation_patient[0], mHeader.image_orientation_patient[1],
                                            mHeader.image_orientation_patient[2], mHeader.image_orientation_patient[3],
                                            mHeader.image_orientation_patient[4], mHeader.image_orientation_patient[5]);

    fprintf(outFile, "invert_x:\t\t\t\t%d\n", mHeader.invert_x);
    fprintf(outFile, "invert_y:\t\t\t\t%d\n", mHeader.invert_y);
    fprintf(outFile, "invert_z:\t\t\t\t%d\n", mHeader.invert_z);
    fprintf(outFile, "swap_xy:\t\t\t\t%d\n", mHeader.swap_xy);
    fprintf(outFile, "swap_yz:\t\t\t\t%d\n", mHeader.swap_yz);
    fprintf(outFile, "swap_zx:\t\t\t\t%d\n", mHeader.swap_zx);
    fprintf(outFile, "version:\t\t\t\t%d\n", mHeader.version);
    fprintf(outFile, "hdr_size:\t\t\t\t%d\n", mHeader.hdr_size);
}
