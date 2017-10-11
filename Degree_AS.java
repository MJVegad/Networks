/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.SortedMap;
import java.util.StringTokenizer;

/**
 *
 * @author mihirvegad
 */
public class Degree_AS {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        
        //String asPathsFile=args[1];
        
        try{
            
            BufferedReader br=new BufferedReader(new FileReader(args[0]));
            Map<String,Set<String>> asNeighbours=new HashMap<String, Set<String>>();  
            
            String row;
            while((row=br.readLine())!=null)
            {
                //To get AS_path in temp
                 StringTokenizer st=new StringTokenizer(row);
                 String temp[]=new String[st.countTokens()+1];
                 
                // System.out.println(st.countTokens());
                 int i=1;
                 while(st.hasMoreTokens())
                 {
                    temp[i]=st.nextToken();
                    i++;
                 }
                 
                 for(int k=1;k<(temp.length-1);k++)
                 {
                     //neighbour[ui]=neighbour[ui] U {u(i+1)}
                   if(asNeighbours.containsKey(temp[k]))
                       asNeighbours.get(temp[k]).add(temp[k+1]);
                   else
                   {
                       Set<String> ases=new HashSet<String>();
                       //To deal with AS Path padding
                       ases.add(temp[k]);
                       ases.add(temp[k+1]);
                       asNeighbours.put(temp[k],ases);
                   }
                   
                   //neighbour[u(i+1)]=neighbour[u(i+1)] U {ui}
                   if(asNeighbours.containsKey(temp[k+1]))
                       asNeighbours.get(temp[k+1]).add(temp[k]);
                   else
                   {
                       Set<String> ases=new HashSet<String>();
                       //To deal with AS Path padding
                       ases.add(temp[k+1]);
                       ases.add(temp[k]);
                       asNeighbours.put(temp[k+1],ases);
                   }
                   
                 }
                 
            }
            
                 Map<String,Integer> finalop = new HashMap<>();
                 Iterator it=asNeighbours.entrySet().iterator();
                 while(it.hasNext())
                 {
                    
                 Map.Entry<String,Set<String>> kv=(Map.Entry<String,Set<String>>)it.next();
                 finalop.put(kv.getKey(), kv.getValue().size());
                 
                 }
                 
                 List<Map.Entry<String,Integer>> list=new ArrayList<Map.Entry<String,Integer>>(finalop.entrySet());
                 
                 Collections.sort(list, new Comparator<Map.Entry<String, Integer>>() {
                     @Override
                     public int compare(Map.Entry<String,Integer> ob1, Map.Entry<String,Integer> ob2)
                     {
                         //To get output in descending order
                         return (ob2.getValue()).compareTo(ob1.getValue());
                     }
                
                    
                 });
                
                 System.out.println("<ASN> <Degree> in descending order of degree.."); 
                 Iterator<Map.Entry<String,Integer>> iter=list.iterator();
                 while(iter.hasNext())
                 {
                     Map.Entry<String,Integer> entry=iter.next();
                     //Assuming As is not neighbour of itself
                     System.out.println(entry.getKey()+" "+(entry.getValue()-1));
                 }
                 
                 
            
            
        }catch(Exception e)
        {
          e.printStackTrace();
        }
        }
    
}
