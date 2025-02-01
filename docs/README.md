# asciify

This is my term paper titled "ASCII-renderer". I cover development of a C++ library for rendering ASCII art as well as Neovim + Telescope integration in it. It works as a regular Neovim plugin and Telescope extension, but it is not realy suitable for everyday use. Especially the way Lua side works: it uses pre-built shared object which means it will work only on linux with the same library versions; it replaces default telescope viewer with custom and uses default only as a fallback which means it tries to open every file it sees as each of it's supported formats and if everything fails, then it will use the default previewer, which makes navigating files sometimes laggy; also it does not support PNGs which is really the most used format. So i don't see why anyone would use the plugin in it's current state. 

Despite all said, this can be a good inspiration or basement for your projects.

And here is the text of the paper itself:

> The following text has been translated [from Russian](README.ru.md) via DeepSeek R1 as i felt too lazy to do it by hand. I believe it completed it's task exceptionally well though.

# INTRODUCTION

This work explores the principles of development and demonstrates the practical capabilities of an ASCII renderer—a software system designed to convert raster images into their corresponding textual representations. Additionally, the integration of the developed ASCII renderer with the Neovim text editor as a plugin for previewing raster images has been implemented.

The development process of the system can be divided into the following key parts:

1. Loading a raster image in a standard format.

2. Converting the raster image into its textual representation.

3. Displaying the resulting textual representation of the original image on the screen (e.g., using a terminal emulator) or saving it to a text file.

The developed system can be used for generating textual representations of raster images for various purposes:

- Creating text-based logos and banners.

- Viewing images in environments with limited display capabilities (e.g., terminal emulators or certain messaging platforms).

- Simply for entertainment.

# 1 LITERATURE REVIEW

## 1.1 Overview of Methods and Algorithms for the Task

Computer graphics is a rapidly evolving field characterized by continuous technological advancements and the emergence of new stylistic trends. At the same time, previously relevant visual styles quickly become outdated. What was once considered the pinnacle of realism may now appear primitive and unaesthetic. However, some styles that have lost their former popularity unexpectedly experience a revival. Prominent examples of this phenomenon include the aesthetics of the Sony PlayStation 1, the visual style of the game Quake, pixel art, and ASCII art. The latter, despite its minimalism, possesses a unique aesthetic appeal and evokes a sense of nostalgia. Emerging from the limited capabilities of early personal computers in the 70s and early 80s, which could only display text, ASCII art has regained popularity as a means of artistic expression and a tool for creating unique visual effects.

The task of synthesizing textual images (ASCII art) can be approached using various methods. The most common approach is applying post-processing effects to existing images. This method offers several advantages, such as independence from specific rendering technologies and the ability to apply it to existing images (e.g., photographs). However, a significant drawback of this approach is the limited flexibility in controlling individual elements of the resulting ASCII art. Despite this limitation, the post-processing method was chosen for this project due to its relative simplicity and the potential to achieve expressive results.

The process of generating ASCII art can be represented as a sequence of the following steps:

1. **Image discretization**: The original image is divided into a regular grid of cells, with each cell corresponding to the size of a character.

2. **Filling process**: Each cell is assigned a character from a given alphabet based on its average brightness.

3. **Edge detection**:

    - 3.1. Image edge detection is performed using specialized algorithms.

    - 3.2. For cells intersected by edges, a character from the given alphabet is selected to reflect the nature of the edge (e.g., slope angle).

4. **Coloring process**: Each cell is assigned a color based on the average color value within it. This value can be derived from the RGB color space or one of the eight ANSI color specifications. Since terminal emulators primarily use ANSI control sequences for coloring, such images are technically ANSI art rather than ASCII art.

It is worth noting that different implementations of ASCII art synthesis algorithms may include various combinations of these steps, leading to significant visual differences in the resulting images.

## 1.2 Overview of Information Technologies for the Task

Traditionally, ASCII art has been used for visualizing images in text-based environments, such as terminal emulators. However, due to its high artistic potential, ASCII art has found applications in other domains. It allows for the creation of unique visual styles and effects, making it a valuable tool for designers and artists.

One of the most popular tools for creating ASCII art is the `aalib` library. It provides developers with a wide range of functions for converting raster images into text formats, supporting various character encodings and allowing for customizable rendering quality. A typical example of `aalib` usage is the VLC media player, which supports a mode for playing videos as ASCII art (see Figure 1.1).

![Figure 1.1 - Example of the VLC media player in ASCII art mode](readme_media/image1.png)

ASCII art has also found its way into the gaming industry. For example, in the video game *Returnal*, an ASCII art effect implemented as a shader for the photo mode allows players to create unique stylized screenshots. Despite its advantages, ASCII art has limitations. Low resolution, limited color range, and the complexity of creating complex 3D scenes are the main obstacles to its widespread use in modern graphical applications.

## 1.3 Problem Statement. Structure of Input and Output Data

This project involves the development of a software system for converting raster images into ASCII art.

**Input data**:

1. A raster image in a standard format (e.g., JPEG or WebP). The WebP format was chosen due to its relative simplicity and support for animated images, potentially allowing for the addition of animated image support. The JPEG format is used to demonstrate support for various formats.

2. Parameters:

   - Fill character palette.

   - Border character palette.

   - Minimum threshold value for border intensity.

   - Additional border parameters for the "Difference of Gaussians" algorithm.

   - Color display mode (no color, standard ASCII colors, or RGB colors).

   - A list of effects to be applied in the specified order.

**Output data**: The ASCII art corresponding to the original image.

**Functional requirements**:

1. Algorithmic conversion of a raster image into its corresponding ASCII art based on the specified parameters.

2. Flexible configuration of conversion parameters to achieve various visual styles.

3. Simplicity and ease of use and configuration.

# 2 FUNCTIONAL DESIGN

The C++ programming language was chosen for this project due to its extensive capabilities in image processing, a rich standard library, and effective support for object-oriented programming (OOP) principles. The high performance of C++, comparable to that of C, allows for efficient solutions to the tasks outlined in this project.

> In reality, the language was chosen by the university curriculum designers. If I had a choice, I would have chosen C for CPU programming, as (in my experience) C++ projects tend to grow unnecessarily complex. Plus, C++ is unaesthetic.

## 2.1 Development of the Class Diagram

The class diagram is a structural UML diagram that illustrates the overall hierarchy of the system's classes, their collaborations, fields, methods, and relationships. The purpose of the class diagram is to provide an overview of the system's structure, identify key objects and their relationships, and serve as a foundation for further system development.

[Class Diagram](class_diagram.pdf)

## 2.2 Class Descriptions

The system's classes can be divided into several categories:

- **General system classes**: Designed for use throughout the system. This category includes, for example, the class for working with images and structures for representing position and color.

- **Classes for working with raster images**: Classes independent of ASCII art, intended solely for working with raster images.

- **Classes for working with ASCII art**: Classes designed for creating, processing, and outputting ASCII art. These may depend on raster image classes.

- **Auxiliary classes for additional system functionality**.

Since C++ is a multi-paradigm programming language, the code does not necessarily have to consist solely of classes. Therefore, other components of the system include modules—groups of one or more source code files with similar functionality that do not contain classes.

Among the system's modules, the user interface module, written in Lua as a plugin for the Neovim text editor, stands out. As a dynamically typed scripting language, Lua provides extensive capabilities for writing configurations and user interfaces.

### 2.2.1 General System Classes

At the core of the system is the templated container class `Image<T>`. The template parameter `T` defines the pixel type, allowing this class to be used in various tasks. The image is stored as a row-major array in the heap. The class provides methods for accessing pixels and size, as well as an iterator `Image<T>::Iterator` for sequential pixel traversal. The nested class `Image<T>::Region` allows for efficient work with subregions of the image.

`Size` and `Pos` are structures representing dimensions and coordinates, respectively. They are used throughout the system's code. The structures store components in 16-bit integer variables, which is suitable for working with images. Additionally, algebraic operators are implemented for convenient geometric calculations (e.g., vector addition and subtraction, comparison).

`Color` is a structure representing a color in the RGB model. Color components are stored as floating-point numbers. Functions for calculating luminance (`luminance()`) and color magnitude (`magnitude()` and `sqr_magnitude()`) are implemented, along with algebraic operators for simplifying color manipulation.

The templated interface `ImageFilter<T>` defines a general contract for filters applied to images. Despite its name, it can be implemented for any type. Filters can perform any transformations on the image that do not change its size. The friendly operator `*=` allows filters to be applied to images intuitively and clearly indicates that the filter may modify the image object.

### 2.2.2 Classes for Working with Raster Images

The `Bitmap` class is the primary class for working with raster images. It inherits from `Image<Color>` and is used to store a sequence of colors in a raster image.

The `BitmapCodec` class is used for decoding (loading) and potentially encoding (saving) images. The static method `BitmapCodec::decode()` loads an image in any supported format using methods from child classes. It also contains two nested error structures inherited from the standard `std::exception`: the abstract `BitmapCodec::DecodingException` (used by child classes to implement their own errors) and `BitmapCodec::UnsupportedFormatException` (thrown when no decoder can decode the image).

Two child classes of `BitmapCodec`—`JpegCodec` and `WebpCodec`—use the `libjpeg` and `libwebp` libraries, respectively, to load specific image formats. Additionally, nested error structures implementing `BitmapCodec::DecodingException`—`JpegCodec::InternalDecodingException` and `JpegCodec::InvalidHeaderDecodingException`—are used for exceptional situations during JPEG image decoding.

The `BitmapFilter` interface inherits from `ImageFilter<Bitmap>` and adds the nested structure `BitmapFilter::CnvlKernel<T, W, H>`, which is key to implementing all specific filters for raster images. It represents a convolution kernel of size WxH, which can be applied to an image to achieve a wide range of visual effects. The structure has two constructors—one based on a matrix and another based on a function—abstracting away the specific kernel. This significantly reduces the code of child filters, and some filters can even be eliminated as unnecessary. The `CnvlKernel` structure stores a matrix of elements of type `T`, which is used in the `BitmapFilter::CnvlKernel::apply()` method to apply the convolution operation to the image. The algorithm of this method is described in detail in subsection 3.2.2.

The `DoGBitmapFilter` and `SobelBitmapFilter` classes, implementing `BitmapFilter`, are used to apply the "Difference of Gaussians" and "Sobel operator" filters, respectively, to an image. The nested class `DoGBitmapFilter::GaussianKernel<S>` simplifies the creation of convolution kernels of size `S`x`S` based on a 2D Gaussian function. These filters form the basis of the edge detection algorithm, described in detail in subsection 3.2.1.

### 2.2.3 Classes for Working with ASCII Art

The `AsciiEl` structure is a universal container for information about an ASCII art element (the character and its color).

The `Ascii` class represents ASCII art itself. Inheriting from `Image<AsciiEl>`, it stores a sequence of special elements. It is created based on the original image (used in filters), the field size in characters, and the character size.

The `AsciiWriter` class is used to output ASCII art to a standard stream or a file (by name). It is created based on the ASCII art and the output field size in characters. A key feature of this class is the presence of an additional parameter in the write methods that determines the coloring mode: no color (useful for sending in text messages without ANSI control sequence support), standard ANSI (using up to eight colors), and the more modern "true color" mode, which allows the use of all 16.7 million RGB colors but may not be supported by older terminal emulators.

The `AsciiFilter` class, implementing `ImageFilter<Ascii>`, exists solely for better code readability and potential future system expansion. Currently, it remains empty.

The system implements three filters, each responsible for a specific characteristic of the resulting image:

- `FillAsciiFilter` (applying fill): Determines the correspondence between the average brightness of regions in the original image and ASCII characters, forming the basis for further processing.

- `ColorAsciiFilter` (applying color): Determines the correspondence between the average color of regions in the original image and ASCII characters.

- `EdgeAsciiFilter` (edge detection): Detects object edges in the image and represents them with corresponding ASCII characters based on their shape (in this case, the slope angle is considered).

Effects can be applied in any order and combination. The compositional approach used offers several advantages:

- Easy system extensibility by adding new effects.

- The ability to apply effects in all possible combinations, which may be challenging with inheritance.

### 2.2.4 Auxiliary Classes and Modules

The `Font` class allows calculating the size of a character in pixels based on a given height in points, which is necessary for maintaining the proportions of the original image when using fonts with arbitrary aspect ratios. The static method `Font::load()` loads a font file using the `libfreetype` library, which provides powerful tools for working with fonts. If an error occurs during font loading, the `Font::InternalLoadingException` error, derived from `std::exception`, is thrown.

The main module `main.cpp` can potentially be used both for implementing a command-line interface and as a stub for future user interface modules. At this stage, the module provides a simple and quick way to demonstrate the capabilities of the ASCII renderer.

The glue module `c_glue.cpp` makes core functions easily accessible from a dynamically linked library. This library is used by the user interface module for easy access to the software interface functions.

### 2.2.5 User Interface Module

This module can be divided into two submodules:

1. **Foreign Function Interface (FFI) and Lua binding for the dynamically linked library**: The key element of accessing the ASCII renderer's functionality from Lua is the FFI mechanism. FFI provides a "bridge" between two languages, allowing functions written in one language to be called in code written in another. In this case, FFI is used to call C++ functions from Lua scripts. For FFI to work, a dynamically linked library exporting the necessary functions for external calls is required. On the C++ side, the necessary functions must be created according to C language rules and marked with the `extern "C"` attribute. This allows the Lua script to precisely identify the function name during the call, avoiding issues related to possible function name mangling by the C++ compiler. To simplify memory management, a metatable (analogous to a parent class in Lua) `Native` and several tables (analogous to classes) are implemented for easy use of the exported ASCII renderer functions in an object-oriented interface.

2. **Extending Telescope's functionality for image preview**: To implement image preview in the Neovim text editor, the popular Telescope extension was chosen. This extension allows interactive searching through various data sources, including files, buffers, and commit history, and performing actions on search results. Telescope is modular, offering extensive customization options. In this project, Telescope was extended with image preview functionality based on the developed ASCII renderer. Using the exported tables, Telescope creates a "previewer"—a table containing a function for previewing files at a given path. In this function, using the aforementioned submodule, the image is loaded; the user's monospaced font is determined; ASCII art is created using the user's effect set and parameters; and the result is output to the buffer received from Telescope. In case of an error or unsupported format, Telescope's default previewer is used. This allows the image previewer to be integrated into Telescope without losing its built-in functionality. To simplify the configuration and use of the new functionality, a special Telescope extension mechanism was used, registering the extension and allowing users to specify desired rendering parameters.

# 3 DEVELOPMENT OF SOFTWARE MODULES

## 3.1 Development of Algorithm Schemas

For the development of algorithm schemas, the two most important methods, `EdgeAsciiFilter::operator()()` and `BitmapFilter::CnvlKernel::apply()`, were chosen, as they are the most complex algorithms in the system.

[Algorithm schema for `EdgeAsciiFilter::operator()()`](flowchart_EdgeAsciiFilter.pdf)

> Since the [original schema](flowchart_CnvlKernel_non_simplified.pdf) did not fit on an A4 sheet, it was simplified to a version that, while less optimized, is easier to understand.

[Algorithm schema for `BitmapFilter::CnvlKernel::apply()`](flowchart_CnvlKernel.pdf)

## 3.2 Development of Algorithms

### 3.2.1 Development of the Algorithm for the `EdgeAsciiFilter::operator()()` Method

This method is responsible for applying the edge detection filter to ASCII art. It implements the virtual method `AsciiFilter::operator()()`, used for applying filters to ASCII art.

The method can be broken down into the following main steps:

1. Copying the original raster image used for the ASCII art to prevent accidental modification.

2. Applying edge detection filters (sequentially "Difference of Gaussians" to improve image characteristics and "Sobel operator" to compute the approximate gradient of the image's brightness) to the copied raster image. The result is a gradient image containing information about the approximate partial derivatives of the image in the horizontal and vertical directions in two channels (red and green).

3. For each element of the ASCII art:

   1. The region of the gradient image corresponding to the current ASCII art element is determined.

   2. For each pixel in the region:

      1. The edge slope angle is computed using the `atan2()` function.

      2. Based on the computed angle, the index of the border character from the given palette is determined (assuming the characters are arranged in order corresponding to the slope angles from 0 to π or from -π to 0).

      3. The weight of the computed edge is added to the array of edge weights.

   3. The edge weight for the current element is computed as the sum of the weights of individual edges. If the resulting value is below the threshold, the current ASCII art element remains unchanged, and the process moves to the next element.

   4. The edge with the maximum weight is determined. The current ASCII art element is overwritten with the character corresponding to this edge.

### 3.2.2 Development of the Algorithm for the `BitmapFilter::CnvlKernel::apply()` Method

This method is responsible for applying an arbitrary convolution kernel to a raster image. To avoid unnecessary copies, it operates on pre-existing images, writing the result of the convolution into one of them.

The method can be broken down into the following main steps:

1. For each pixel in the source image:

   1. The array of source pixels is filled from the area around the current position.

   2. The weighted sum of the pixels in the area is computed as the sum of the products of the source pixels and their corresponding kernel elements.

   3. The sum is written to the resulting image.

It is worth noting that the above algorithm involves many redundant image reads. This can be easily prevented by slightly changing the data storage sequence. Since the image is stored as a sequence of rows, the best approach is to traverse along the rows and store the kernel and source pixel array as a sequence of columns. This allows for sequential pixel traversal and shifting columns along the row instead of repeatedly reading from the image.

# 4 PROGRAM RESULTS

As a result of the program's operation as a plugin for the Neovim text editor, the Telescope file search window can be seen, where the developed plugin is used as the previewer for raster image files (Figure 4.1).

![Figure 4.1 - Program result as a Telescope extension, a plugin for the Neovim text editor](readme_media/image2.png)

As a result of the program's operation in quick demonstration mode, separate from the developed plugin, the conversion of a WebP format raster image (Figure 4.2, a) into ASCII art (Figure 4.2, b) and its output to a terminal emulator using standard ANSI colors can be seen. The program demonstration takes place in the xterm terminal emulator.

|   |
|---|
|![Figure 4.2, a - original image](readme_media/image3.png)|
|![Figure 4.2, b - corresponding ASCII art](readme_media/image4.png)|

# CONCLUSION

The research and development of the ASCII renderer have resulted in a flexible and efficient tool for converting raster images into their textual counterparts. The implemented system offers extensive customization options and allows for the creation of ASCII art that can be used for various purposes, from creating unique text-based logos to data visualization.

The project developed a flexible object-oriented system for converting raster images into ASCII art. The system is based on the principles of data and method encapsulation in classes, class inheritance, and polymorphism, enabling an extensible architecture. Class templates provided system universality, while the class hierarchy organized the code into logical groups and simplified maintenance.

The use of object-oriented design patterns allowed for the creation of a modular system where new functionality can be easily added without significant rework of existing code.

The developed ASCII renderer offers flexible configuration, allowing users to choose various filters and adjust their parameters. This makes the tool versatile and suitable for solving a wide range of tasks related to image-to-text conversion and visual effects creation.

Through this project, the principles of OOP and various computer graphics algorithms were successfully studied. The knowledge and skills gained in this project can be applied in the future to develop more complex computer graphics systems.

# REFERENCES

\[1\]. Character representation of grey scale images \[Online\]. - 1997 - Available: https://paulbourke.net/dataformats/asciiart/ - Accessed: 25.10.2024.

\[2\]. Document winnemoeller-cag2012.pdf \[Online\]. - 2012 - Available: https://users.cs.northwestern.edu/~sco590/winnemoeller-cag2012.pdf - Accessed: 26.10.2024.

\[3\]. Document example.c \[Online\]. - 2009 - Available: https://raw.githubusercontent.com/LuaDist/libjpeg/refs/heads/master/example.c - Accessed: 03.11.2024.

\[4\]. WebP API Documentation | Google for Developers \[Online\]. - 2024 - Available: https://developers.google.com/speed/webp/docs/api - Accessed: 03.11.2024.

\[5\]. FreeType Tutorial / I \[Online\]. - 2024 - Available: https://freetype.org/freetype2/docs/tutorial/step1.html - Accessed: 05.11.2024.

\[6\]. nvim-telescope/telescope.nvim: Find, Filter, Preview, Pick. All lua, all the time. \[Online\]. - 2024 - Available: https://github.com/nvim-telescope/telescope.nvim - Accessed: 10.11.2024.

\[7\]. telescope.nvim/developers.md at master \[Online\]. - 2024 - Available: https://github.com/nvim-telescope/telescope.nvim/blob/master/developers.md - Accessed: 10.11.2024.

\[8\]. I Tried Turning Games Into Text \[Video\]. - 2024 - Available: https://www.youtube.com/watch?v=gg40RWiaHRY - Accessed: 11.11.2024.
