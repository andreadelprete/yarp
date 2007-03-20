// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2006 Paul Fitzpatrick, Giorgio Metta
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

//
// $Id: DragonflyDeviceDriver.h,v 1.29 2007-03-20 11:29:32 eshuy Exp $
//
//

#ifndef __DragonflyDeviceDriverh__
#define __DragonflyDeviceDriverh__

//=============================================================================
// YARP Includes
//=============================================================================

// May 06, readapted for YARP2 by nat

#include <yarp/os/Semaphore.h>
#include <yarp/os/Bottle.h>
#include <yarp/dev/FrameGrabberInterfaces.h>

namespace yarp {
    namespace dev {
        class DragonflyOpenParameters;
        class DragonflyDeviceDriver;
    }
}

/**
 * \file DragonflyDeviceDriver.h device driver for managing the 
 * Dragonfly IEEE-1394 Camera (WINNT version)
 */

/**
 * Structure for defining the open() parameters of the camera.
 */
class yarp::dev::DragonflyOpenParameters
{
public:
	// Parameters
	unsigned int _unit_number;
	unsigned int _size_x;
	unsigned int _size_y;
	unsigned int _video_type;

	double _whiteR;
	double _whiteB;

	double _brightness;
	double _exposure;
	double _shutter;
	double _gain;

	bool _fleacr;  //FLEA color reconstruction flag

	/**
	 * Constructor. Add here the parameters for the open().
	 */
	DragonflyOpenParameters()
	{
		// parameters initialization
		_unit_number = 0;
		_size_x = 640;
		_size_y = 480;
		_video_type = 0;

		//uninitialized - inherit registry stored values
		_brightness=-1;
		_exposure=-1;
		_shutter=-1;
		_gain=-1;
		_whiteR=-1;
		_whiteB=-1;


		// FLEA cameras are compatible with DRAGONFLY's but ...
		// the color reconstruction method is different 
		// (GBRG instead of RGGB)
		// The default is to use Dragonsfly's method
		_fleacr = false;

	}

};

/**
 * @ingroup dev_impl_media
 *
 * A Point Grey Dragonfly digital camera (or, on Linux, any digital camera).
 */
class yarp::dev::DragonflyDeviceDriver : 
    public IFrameGrabber, public IFrameGrabberRgb, public IFrameGrabberImage, public IFrameGrabberControls, public DeviceDriver
{
private:
	DragonflyDeviceDriver(const DragonflyDeviceDriver&);
	void operator=(const DragonflyDeviceDriver&);

public:
	/**
	 * Constructor.
	 */
	DragonflyDeviceDriver();

	/**
	 * Destructor.
	 */
	virtual ~DragonflyDeviceDriver();

    // temp: here for debug purposes only
    void recColorFSBilinear(const unsigned char *src, unsigned char *out);
    void recColorFSNN(const unsigned char *src, unsigned char *out);
    void recColorHSBilinear(const unsigned char *src, unsigned char *out);


    /**
	 * Open the device driver.
     * @param par parameters for the device driver
	 * @return returns true on success, false on failure.
	 */
    bool open(const DragonflyOpenParameters& par);

    virtual bool open(yarp::os::Searchable& config)
	{
        DragonflyOpenParameters params;
		yarp::os::Value *value;
		if (config.check("unit_number",value)||config.check("d",value)) {
			params._unit_number = value->asInt();
		}
		if (config.check("size_x",value)||config.check("width",value)){
			params._size_x  = value->asInt();
		}
		if (config.check("size_y",value)||config.check("height",value)){
			params._size_y  = value->asInt();
		}
		params._video_type = config.find("video_type").asInt();

		//params._offset_y = config.find("offset_y").asInt();
		//params._offset_x = config.find("offset_x").asInt();
		//params._alfa = (float)config.find("alfa").asInt();
		yarp::os::Bottle& whites = config.findGroup("white_balance");
		if (!whites.isNull()) {
			params._whiteR = whites.get(1).asDouble();
			params._whiteB = whites.get(2).asDouble();
		}
	
		if (config.check("brightness", value)){
			params._brightness=value->asDouble();
		}
		if (config.check("exposure", value)){
			params._exposure=value->asDouble();
		}
		if (config.check("shutter", value)){
			params._shutter=value->asDouble();
		}
		if (config.check("gain", value)){
			params._gain=value->asDouble();
		}

		params._fleacr = config.check("flea", "If present indicates to use Flea color reconstruction ");
		

		return open(params);
    }

	/**
	 * Closes the device driver.
	 * @return returns true/false on success/failure.
	 */
	virtual bool close(void);

    /**
     * Implements FrameGrabber basic interface.
     * @param buffer the pointer to the array to store the last frame.
     * @return returns true/false on success/failure.
     */
    virtual bool getRawBuffer(unsigned char *buffer);

    /**
     * Implements the Frame grabber basic interface.
     * @return the size of the raw buffer (for the Dragonfly
     * camera this is 1x640x480).
     */
    virtual int getRawBufferSize();

    /**
     * Implements FrameGrabber basic interface.
     */
    virtual int height() const;
    
    /**
     * Implements FrameGrabber basic interface.
     */
    virtual int width() const;

    /** 
     * FrameGrabber bgr interface, returns the last acquired frame as
     * a buffer of bgr triplets. A demosaicking method is applied to 
     * reconstuct the color from the Bayer pattern of the sensor.
     * @param buffer pointer to the array that will contain the last frame.
     * @return true/false upon success/failure
     */
    virtual bool getRgbBuffer(unsigned char *buffer);

    /** 
     * FrameGrabber image interface, returns the last acquired frame as
     * an rgb image. A demosaicking method is applied to 
     * reconstuct the color from the Bayer pattern of the sensor.
     * @param image that will store the last frame.
     * @return true/false upon success/failure
     */
    virtual bool getImage(yarp::sig::ImageOf<yarp::sig::PixelRgb>& image);

    /**
     * Set image normalized brightness [0.0 : 1.0].
     */
    virtual bool setBrightness(double v);

    /**
     * Set normalized shutter time [0.0 : 1.0].
     */
    virtual bool setShutter(double v);

    /**
     * Set normalized gain [0.0 : 1.0].
     */
    virtual bool setGain(double v);

    /**
     * Get normalized shutter time [0.0 : 1.0].
     */
    virtual double getShutter() const;

    /**
     * Get normalized gain [0.0 : 1.0].
     */
    virtual double getGain() const;

    /**
     * Get normalized image brightness [0.0 : 1.0].
     */
    virtual double getBrightness() const;

	virtual bool setWhiteBalance(double red, double green);

	virtual bool getWhiteBalance(double &red, double &green) const;

protected:
	void *system_resources;
};

/**
 * @ingroup dev_runtime
 * \defgroup cmd_device_dragonfly dragonfly

 A streaming digital camera source, see yarp::dev::DragonflyDeviceDriver.

*/


#endif
