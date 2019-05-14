#!/bin/bash
IFS=$'\n'

echo "args = $#"
echo "Reading    " "$1"
echo "Writing to " "$2"
echo "Performing " "$4"
if [ $# -ge 3 ];then
        # Lets check if input directory exists
        if [ -d "$1" ];then
                pushd "$1"
        else
                echo "input must be directory"
                exit 0
        fi
        # Lets check if output directory exists
        if [ ! -d "$2" ];then
            echo "output must be directory"
            exit 0
        fi

        if [ "$3" = "tomp3" ];then
            todo="$3"
            echo -e "\nConverting to mp3\n"
        else
            # Lets validate the image
            if [ ! -f "$3" -o "$3" == "" ];then
                    echo "video image must be a file jpg/jpeg/png/gif";
                    exit 0
            else
                logo="$3"
                todo="$4"
                echo -e "\nConverting to mp4\n"
            fi
        fi

else
    echo "Usage: convertor [output] [input] [video image]"
    exit 0
fi

vids="$2" #(echo `pwd`/vids)

if [ ! -d $vids ];then
        mkdir -p $vids
fi

function doStuff(){
        sleep 1s
        if [ ! -d "$2" ];then
            mkdir -p "$2"
        fi
        echo "looping in \"$1\""
        for i in `ls "$1"`; do
                if [ "$i" == "vids" ];then continue;fi
                if [ -d $i ]; then
                        #vids=$vids/$i
                        echo "transerving into $i";
                        pushd $i;
                                #sleep 3s;
                                doStuff "$1"/$i "$2"/$i  # If its a directory, we have to transverse it
                        popd
                fi
                # at this point, we are in the directory and need to convert
                if ! ls | grep .jpg; then
                        #cp $logo .
                        echo "";
                fi
                #start converting

                if [ "$todo" = "tomp3" ];then
                    echo "running";
                    if echo $i | grep -e .mp4 -e .mkv ; then
                            # lets first check if its beeng converted before
                            mp3=`echo $i | awk -F ".mp4" '{ print $1 }'`.mp3
                            if ls "$2" | grep $mp3; then continue;fi
                            echo ":" "$i" "$2"/"$mp3"
                            /home/nhlaluko/Desktop/ffmpeg -i "$i" "$2"/"$mp3"
                            echo -e "\n-----------------------------------NEXT-------------------------------------------\n"
                    fi
                else
                    #img=`ls | grep .jpg`
                    #img=`echo $img | awk -F " " '{ print "$1" }'`
                    #img=$logo
                    # Lets check if current file is mp3
                    if echo $i | grep -e .mp3 -e .wmazzzzzzz ; then
                            # lets first check if its been converted before
                            mp4=`echo $i | awk -F ".mp3" '{ print $1 }'`.mp4
                            if ls "$2" | grep $mp4; then continue;fi
                            #if echo $i | grep .wma;then
                                    #mp3=`echo $i | awk -F ".wma" '{print "$1"}'`.mp3
                                    #ffmpeg -i $i  -acodec libmp3lame -ab 192k $mp3
                                    #$i=$mp3
                            #fi
                            #pushd $1
                            #echo ffmpeg -y -loop 1 -framerate 1 -i $img -i \"$i\" -c:v libx264 -preset veryslow -crf 0 -c:a copy -shortest \"$2\/$mp4\"
                            /home/nhlaluko/Desktop/ffmpeg -y -loop 1 -framerate 1 -i "$img" -i "$i" -c:v libx264 -preset veryslow -crf 0 -c:a copy -shortest "$2"/"$mp4"
                            echo -e "\n-----------------------------------NEXT-------------------------------------------\n"
                            #popd
                    fi
                fi
        done
        #echo "---------: vids is back "$1"";
}
sleep 1s
doStuff "$1" "$2" #vids
