import java.util.*;
import java.lang.Math;

public class Random_Track_Picker {
    
    public static void main(String[] args) {
        int album_l = 0, min = 0, max = 0, out, hl = 0, lst = 0, bl = 0;
        String ans;
        Scanner scn = new Scanner(System.in);
        System.out.println("Pick a Track?");
        ans = scn.nextLine();
        if(ans.matches("No") && ans.matches("no") && ans.matches("No.") && ans.matches("no.")) ans = "end";
        if(ans != "end"){
            System.out.println("Pick album length");
            album_l = scn.nextInt();
            System.out.println("Pick a min");
            min = scn.nextInt();
            System.out.println("Pick a max");
            max = scn.nextInt();
            scn.nextLine();
        }
        while(!ans.matches("end") && !ans.matches("End") && !ans.matches("end.") && !ans.matches("End.")){
            out = ((int)(Math.random()*album_l)%album_l+min);
            if(bl == out)continue;
            if(lst == out|| (lst != 0 && out == (lst+1)) || (((out == 1 && lst == 7) || (lst == 1 && out == 7))))continue;
            if(hl == out|| (hl != 0 && out == (hl+1)) || (hl != 0 && out == (hl-1)) || (((out == 1 && hl == 7) || (hl == 1 && out == 7))))continue;
            System.out.println("==================");
            System.out.println(out);
            System.out.println("------------------");
            System.out.println("Pick a Track?");
            ans = scn.nextLine();
            if(lst != 0)bl = lst;
            lst = hl;
            hl = out;
        }

        scn.close();
    }
}
