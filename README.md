# Real-time filtering

<b>Note</b>: Report can be found in the `report/` directory under the name `jake_van_meter_p1_report.pdf`.

## Links

- <a href="https://docs.opencv.org/4.x/index.html">OpenCV 4.8.0-dev documentation</a>

- <a href="https://learn.microsoft.com/en-us/windows/win32/medfound/video-fourccs">Microsoft documentation on FOURCCs</a>

## Operating System and IDE

- <b>Operating System</b>: Ubuntu 22.04 (Jammy Jellyfish)

- <b>IDE</b>: VSCode and Vim

## How to run

1. `cd` into the project directory (i.e. the directory that this `README.md` file is in).

2. Run `make` to compile the `imgDisplay` and `vidDisplay` executables.

3. run `./bin/imgDisplay` to run the image display program.

4. run `./bin/vidDisplay` to run the video display program.

## Current filters

After running `./bin/vidDisplay`, filters can be toggled on and off by pressing
specific keys. The currently implemented filters with their associated keys
are as follows.

<b>Note</b>: While running the program, press `s` at any time to save the
image. If a filter is enabled, both the filtered and unfiltered version will
be saved in this project directory.

1. Greyscale: `g` or `h` (two different implementations)

2. 5x5 Gaussian Blur: `b`

3. Sobel X: `x`

4. Sobel Y: `y`

5. Gradient Magnitude: `m`

6. Blur and Quantize: `l`

7. Cartoon: `c`

8. Brightness: `t` (must be pressed 6 times to restore to default)

9. Sepia: `e`

