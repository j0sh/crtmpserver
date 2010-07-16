This folder contains only media files and their corresponding meta/seek files
This folder should have read/write access. 

*.seek files are used for fast seek
*.meta files are used like a chahe: instead of trying to parse the meta directly from file, it will be read from the meta file.

Both *.seek and *.meta files are discardable. Hence, you can delete them anytime.
They will be automatically generated when missing
 
The media files should have read access and this directory must have write access 
(for creating the seek/meta files)


Supported media files: flv, mp3, mp4, m4a, m4v, mov, f4v

When a faulty file is discovered at run-time, it will be
automatically moved to file_name.type.bad. 

PLEASE send me those files whenever possible because I want
to improve support for them. You can upload them here:
http://www.rtmpd.com/wiki/Bad%20Media%20Files

Accessing format:
+------+--------------------+----------------+
| type | naming             | target file    |
+======+====================+================+
| mp3  | mp3:file_name      | file_name.mp3  |
+------+--------------------+----------------+
| flv  | file_name          | file_name.flv  |
+------+--------------------+----------------+
| mp4  | mp4:file_name.mp4  | file_name.mp4  |
+------+--------------------+----------------+
| m4a  | mp4:file_name.m4a  | file_name.m4a  |
+------+--------------------+----------------+
| m4v  | mp4:file_name.m4v  | file_name.m4v  |
+------+--------------------+----------------+
| mov  | mp4:file_name.mov  | file_name.mov  |
+------+--------------------+----------------+
| f4v  | mp4:file_name.f4v  | file_name.f4v  |
+------+--------------------+----------------+

