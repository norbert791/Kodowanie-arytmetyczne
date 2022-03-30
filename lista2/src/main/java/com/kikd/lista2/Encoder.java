package com.kikd.lista2;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Scanner;

public class Encoder {
    private long[] startingModel;
    private long  startingLength;
    private long wordSize;

    public void setStartingModel(long[] startingModel) {
        this.startingModel = startingModel;
        this.startingLength = 0;
        for (long el : this.startingModel) {
            startingLength += el;
        }
        wordSize = startingLength;
    }

    public void encode(String inputFileName, String outputFileName) throws IOException {
            BufferedOutputStream outPutStream = new BufferedOutputStream(
                    new FileOutputStream(new File(outputFileName)));
            byte[] data = Files.readAllBytes(Path.of(inputFileName));

    }

    public static void main (String[] args) {

    }
}
