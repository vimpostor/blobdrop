complete -c blobdrop -s '-h' -l 'help' -d 'show help'
complete -c blobdrop -s '-v' -l 'version' -d 'show version'
complete -c blobdrop -s '-f' -l 'frameless' -d 'show frameless window'
complete -c blobdrop -s '-k' -l 'keep' -d 'keep dropped files'
complete -c blobdrop -s '-l' -l 'link' -d 'print OSC8 hyperlinks'
complete -c blobdrop -s '-n' -l 'notification' -d 'send notification'
complete -c blobdrop -s '-p' -l 'persistent' -d 'disable autohiding during drag'
complete -c blobdrop -s '-t' -l 'ontop' -d 'keep window on top'
complete -c blobdrop -s '-x' -l 'auto-quit' -d 'autoquit behaviour' -x -a "never\t'do not autoquit' first\t'after first drag' all\t'after all items have been dragged'"
