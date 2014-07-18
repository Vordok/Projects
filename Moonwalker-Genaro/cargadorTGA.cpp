#include "cargadorTGA.h"

CTga::CTga()
{
	//StateVars = new Globales;
	texID=0;
}

CTga::~CTga()
{
	Elimina();
}

int CTga::freeData()
{
	
	if(imageData != 0)
	{
		delete [] imageData;
	}
	
	return 0;
}

void CTga::Elimina()
{
	if(texID)
    {
         texID--;
         glDeleteTextures(1, &texID);
         texID=0; 
    }

}

int CTga::LoadTGA(char *filename)				// Loads A TGA File Into Memory
{    
	
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};		// Uncompressed TGA Header
	GLubyte		TGAcompare[12];									// Used To Compare TGA Header
	GLubyte		header[6];										// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;									// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;										// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;											// Temporary Variable
	GLuint		type=GL_RGBA;									// Set The Default GL Mode To RBGA (32 BPP)
	

	FILE *file = fopen(filename, "rb");							// Open The TGA File

	if(	file==NULL ||											// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
	{
		if (file == NULL)										// Did The File Even Exist? *Added Jim Strong*
			return FALSE;										// Return False
		else													// Otherwise
		{
			fclose(file);										// If Anything Failed, Close The File
			return FALSE;										// Return False
		}
	}

	width  = header[1] * 256 + header[0];				// Determine The TGA Width	(highbyte*256+lowbyte)
	height = header[3] * 256 + header[2];				// Determine The TGA Height	(highbyte*256+lowbyte)
    
 	if(	width	<=0	||									// Is The Width Less Than Or Equal To Zero
		height	<=0	||									// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))						// Is The TGA 24 or 32 Bit?
	{
		fclose(file);											// If Anything Failed, Close The File
		return FALSE;											// Return False
	}

	bpp	= header[4];								// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= bpp/8;							// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= width*height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	imageData=(GLubyte *)malloc(imageSize);			// Reserve Memory To Hold The TGA Data

	if(	imageData==NULL ||								// Does The Storage Memory Exist?
		fread(imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(imageData!=NULL)							// Was Image Data Loaded
			free(imageData);							// If So, Release The Image Data

		fclose(file);											// Close The File
		return FALSE;											// Return False
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)			// Loop Through The Image Data
	{															// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=imageData[i];								// Temporarily Store The Value At Image Data 'i'
		imageData[i] = imageData[i + 2];		// Set The 1st Byte To The Value Of The 3rd Byte
		imageData[i + 2] = temp;						// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);												// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texID);						// Generate OpenGL texture IDs
	

	glBindTexture(GL_TEXTURE_2D, texID);				// Bind Our Texture
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	
	
	
	if (bpp==24)										// Was The TGA 24 Bits
	{
		type=GL_RGB;											// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);

	freeData();

	return true;												// Texture Building Went Ok, Return True
}