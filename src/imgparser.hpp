#ifndef IMGPARSER_HPP
#define IMGPARSER_HPP

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "str.hpp"
#include "preprocessing.hpp"

using namespace cv;
using namespace std;

// Utility class to derive for parsing all images in a specified folder

class ImgParser
{
private:
	int current;
	Str folder;
	Str extension;
public:
	ImgParser() : current(0), folder("none"), extension("jpg")
	{
	}
	virtual ~ImgParser()
	{
	}
protected:
	bool DigFolder(const Str& path, Mat& mat)
	{
		Str newFolder(path);
		newFolder.Replace('\\', '/');
		if (newFolder.letters[newFolder.length] == '/')
			newFolder.Remove(1);

		if (folder != newFolder)
		{
			current = 0;
			folder = newFolder;
		}

		Str imgPath;

		imgPath << folder << '/' << current << '.' << extension;
		cout << "Loading img \"" << imgPath << "\"" << endl;
		imgPath.AddEndStringChar();
		mat = imread(imgPath.Get(), 1);
		imgPath.RemoveEndStringChar();
		current += 1;

		if (mat.data)
			return true;
		return false;
	}
};

#endif