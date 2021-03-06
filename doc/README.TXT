The eLua doc is automatically generated by a Lua script contained here in the distro.
The html doc generated by this script is the exactly the same shown in our online site (www.eluaproject.net)

To generate the doc:
- make sure you have Lua installed in your machine (www.lua.org)
- make sure you have 'lfs' (Lua file system) and 'md5' modules installed on your machine. To install them:
  * in Windows: the simplest way is to install Lua for Windows (http://code.google.com/p/luaforwindows/) which comes with these modules installed.
    You also need to install source-highlight (for example from http://gnuwin32.sourceforge.net/)
  * in Ubuntu:
    1. install luarocks
      $ sudo apt-get install luarocks
    2. install both modules using luarocks
      $ sudo luarocks install lfs md5
    3. install source-highlight
      $ sudo apt-get install source-highlight
- make sure you have Python installed and the Python installation directory is in your PATH (you might have to add it manually in Windows).
  Python is needed by AsciiDoc (below).
- make sure you have AsciiDoc (http://www.methods.co.nz/asciidoc/INSTALL.html) installed. If you're using Windows you have a bit more work to do:
  * make sure that the AsciiDoc installation directory is in your PATH
  * create a file named "asciidoc.bat" in the AsciiDoc instalation directory with the following line in it:
    @python "c:\Program Files\asciidoc-8.6.2\asciidoc.py" %*
  (substitute "c:\Program Files\asciidoc-8.6.2" with you AsciiDoc instalation directory).
- go to the /doc directory of this distro (you're probably here already :)
- run the script with
  $ lua buildall.lua
- the doc/site structure is generated in a new sub-folder called dist, where you can go and open any .html document. The left-side menu is actually 
  part of the global layout and is included in every page of the doc/site.
- enjoy eLua !

