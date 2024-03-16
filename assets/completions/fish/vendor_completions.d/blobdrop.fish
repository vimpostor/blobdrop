complete -c blobdrop -s '-h' -l 'help' -d 'show help'
complete -c blobdrop -s '-v' -l 'version' -d 'show version'
complete -c blobdrop -s '-b' -l 'frameless' -d 'show frameless window'
complete -c blobdrop -s '-c' -l 'cursor' -d 'spawn window at mouse cursor'
complete -c blobdrop -s '-f' -l 'frontend' -d 'selects frontend' -x -a "auto\t'automatic' gui\t'show window' immediate\t'drag immediately' notify\t'drag from notification' clipboard\t'copy to clipboard' stdout\t'print OSC8 link'"
complete -c blobdrop -s '-i' -l 'intercept' -d 'intercept another DnD'
complete -c blobdrop -s '-k' -l 'keep' -d 'keep dropped files'
complete -c blobdrop -s '-p' -l 'persistent' -d 'disable autohiding during drag'
complete -c blobdrop -s '-R' -l 'remote' -d 'enable ssh remote transparency'
complete -c blobdrop -s '-t' -l 'ontop' -d 'keep window on top'
complete -c blobdrop -s '-x' -l 'auto-quit' -d 'autoquit behaviour' -x -a "never\t'do not autoquit' first\t'after first drag' all\t'after all items have been dragged'"
