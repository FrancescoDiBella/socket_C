/*
    sulla port 3333 un server riceve e memorizza tutte le stringe che gli
    arrivano da un client(fino a 100)
    in risposta si mandano tutte  le stringhe inviate fino a quel momento
    se le stringhe sono 5 allora si restituisce la stringa "LIMITE RAGGIUNTO"
    se la stringa ricevuta è RESET si azzera la lista e risponde OK
*/

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.*;
import java.util.ArrayList;


public class Main{
    public static void main(String[] args) throws IOException{
        System.out.println("Hello World");

        ArrayList<String> bufferList = new ArrayList<>();
        int flag = 1;
        ServerSocket serverSocket = new ServerSocket(3333);
        Socket clientSocket = serverSocket.accept();
        while(flag>0){
            BufferedReader reader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            PrintWriter writer = new PrintWriter(new OutputStreamWriter(clientSocket.getOutputStream()));

            String request = reader.readLine();

            if(request.equals("RESET")){
                bufferList.clear();
                flag = 0;
            }else{
                bufferList.add(request);
                StringBuilder respoStringBuilder = new StringBuilder();

                for(String str : bufferList){
                    respoStringBuilder.append(str);
                }

                if(bufferList.size() >= 5){
                    writer.write("LIMITE RAGGIUNTO");
                } else writer.write(respoStringBuilder.toString());
            }
            writer.flush();
        }
        clientSocket.close();
        

    }
}