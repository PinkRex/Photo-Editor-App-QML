# Photo Editor App

A simple photo editing tool.  
**Developed by:** Nguyễn Minh Đức  
This application was created as a mini project for the **VDT2025** program.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [For Developers](#for-developers)
- [Usage](#usage)

### Features

- Open images from your folder by selecting or dragging and dropping files.
- Capture your current screen and load it into the app.
- Save the edited image to your local folder.
- Zoom in and out.
- Undo, redo, and reset changes.
- Rotate image left or right.
- Flip image vertically or horizontally.
- Resize image.
- Crop image.
- Adjust image brightness.
- Load and unload plugins at runtime.
- Enable and disable plugins.
- Apply plugin effects via custom UI or directly.
- Keyboard shortcuts for basic features.
- Built-in action logger.

### Installation

> ⚠️ Currently, this app only supports the Xorg display server. The Wayland display server is not yet supported.

**Pre-requisites:**
- Qt Creator installed on your machine. Follow this [tutorial](https://www.youtube.com/watch?v=sjApF6qnyUI&t=804s).
- OpenCV 4.x+ installed. See this [tutorial](https://www.youtube.com/watch?v=Nn5OfJjBGmg&t=2s).
- Link Qt with OpenCV. You can follow this [guide](https://www.youtube.com/watch?v=yVeXXuGnnxs&list=PLh0cogPqXcJNEhszlaONshdubdDOF8GRh&index=1&t=4s).

### For Developers

Clone this repo:
```bash
git clone https://github.com/PinkRex/Photo-Editor-App-QML.git
```

1. Open Qt Creator  
2. Locate `CMakeLists.txt` and open it  
3. Build and run the app

### Usage

#### Open File

- Click the "Open" button and choose a file or drag and drop one into the app.

#### Snip Screen

- Click the "Snip" button to enter capture mode.  
- Click on the screen to capture the current display.

#### Change Image View

- Available only when an image is open.  
- Use the zoom in/out buttons to scale the image.

#### Rotate Image

- Available only when an image is open.  
- Use the rotate left/right buttons.  
- Each click rotates the image 90 degrees in the respective direction.

#### Flip Image

- Available only when an image is open.  
- Use the vertical or horizontal flip buttons.  
- The image will flip along the X or Y axis accordingly.

#### Resize Image

- Available only when an image is open.  
- Click the "Resize" button.  
- A dialog will appear with the current image dimensions.  
- Enter new width/height and click Apply or Cancel.

#### Crop Image

- Available only when an image is open.  
- Click the "Crop" button.  
- A dialog will appear with instructions.  
- After closing the dialog, a red boundary will appear around the image.  
- You can resize the boundary but not move it outside the image area. It will auto-snap inside.  
- Click the "Crop" button again to crop the image.

#### Adjust Brightness

- Click the "Adjust Brightness" button.  
- A slider will appear below the button.  
- Adjust the slider to change brightness in real time.

#### Plugin Manager

- Load or unload plugins at runtime.  
- Enable or disable plugins at runtime.

#### Image History

- Available only when an image is open.  
- Each edit pushes the previous state onto the undo stack.  
- Clicking undo pushes the current state onto the redo stack.  
- Click "Reset" to clear both stacks and restore the original image.

#### Actions Logger

- Every action is logged in the right panel.  
- You can clear the log, copy it to the clipboard, or export it as a `.txt` file in the same directory as the executable.

#### Shortcuts

| Shortcut           | Description                            |
|--------------------|----------------------------------------|
| `Ctrl + O`         | Open file dialog                       |
| `Ctrl + S`         | Save the current image                 |
| `Ctrl + Shift + S` | Enter screen capture mode              |
| `Shift + Escape`   | Exit the app                           |
| `Ctrl + +`         | Zoom in                                |
| `Ctrl + -`         | Zoom out                               |
| `Ctrl + Z`         | Undo                                   |
| `Ctrl + Shift + Z` | Redo                                   |
| `Left Arrow`       | Next image in the same folder          |
| `Right Arrow`      | Previous image in the same folder      |
