This QT based application does batch conversions from mp3 to mp4 and vice-versa

Created this because I frequently upload a lot of videos and need to convert them first

So... With that said, how does it work you ask?

- Provide a root directory with all your mp3/mp4

- Set the destination directory

- Select the operation you want (MP3 to MP4 or the other one)

- The application will transverse the directory you provided and recreate
the directory structure in the destination you chose and store all your converted media
in their respective directories

NOTES
- So far it is in testing and ffmpeg is not provided by default
- Edit path in mainwindow.cpp to your ffmpeg library

- I intend to let it use the system installed ffmpeg, but so far ill stick with this for
testing purposes.

Does it work now?
- Yes, it works just fine, but areas of optimazations are alot.
