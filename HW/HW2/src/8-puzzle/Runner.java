import java.io.IOException;
import jigsaw.Jigsaw;
import jigsaw.JigsawNode;

public class Runner {
	/**测试脚本
	 * 分别利用最陡上升爬山法、首选爬山法、随机重启爬山法、模拟退火算法
	 * 求解随机3*3拼图（8-数码问题）
	 * 注意：运行前要修改节点维数，将JigsawNode类中的dimension改为3
	 * 要求：不修改脚本内容，程序能够运行，且得出预期结果
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		
		// 检查节点维数是否为3
		if(JigsawNode.getDimension() != 3){
			System.out.print("节点维数不正确，请将JigsawNode类的维数dimension改为3");
			return;
		}
		
		// 生成目标状态对象destNode: {9,1,2,3,4,5,6,7,8,0}
		JigsawNode destNode = new JigsawNode(new int[]{9,1,2,3,4,5,6,7,8,0});   		

		double testcase = 1000;
		//成功次数的计数
		int suc1, suc2, suc3, suc4;
		suc1 = suc2 = suc3 = suc4 = 0;
		//搜索耗散的计数
		double p1, p2, p3, p4;
		p1 = p2 = p3 = p4 =0;
		
		for (int i = 0; i < testcase; i++) {
			// 生成随机初始状态对象startNode：将目标状态打散，生成可解的随机初始状态
			System.out.println("正在进行第"+ i +"个测试...");
            JigsawNode startNode = Jigsaw.scatter(destNode, 1000);	
			Jigsaw jigsaw = new Jigsaw(startNode, destNode);

			//最陡上升爬山算法
			if (jigsaw.M_HillClimb_Search()) {
				suc1++;
				p1 += jigsaw.getSearchedNodesNum();
			} 

			//首选爬山算法
			if (jigsaw.F_HillClimb_Search()) {
				suc2++;
				p2 += jigsaw.getSearchedNodesNum();
			} 

			//模拟退火算法
			if (jigsaw.S_annealing_Search()) {
				suc4++;
				p4 += jigsaw.getSearchedNodesNum();
			} 
			
			//随机重启爬山算法,写在最后，因为它会改变初始节点
			if (jigsaw.R_HillClimb_Search()) {
				suc3++;
				p3 += jigsaw.getSearchedNodesNum();
			} 
		}
		System.out.println("\n--------------------------------测试结果----------------------------------\n");
		System.out.println( "算法             解决率             耗散\n");
    if (suc1!= 0)
        System.out.printf("最陡爬山法:     %f             %f\n", suc1/testcase, p1/suc1);
    else
        System.out.printf("最陡爬山法:      %f            NAN\n", suc1/testcase);

    if (suc2!= 0)
        System.out.printf("首选爬山法:     %f             %f\n", suc2/testcase, p2/suc2);
    else
        System.out.printf("首选爬山法:     %f             NAN\n", suc2/testcase);

    if(suc3!= 0) 
        System.out.printf("随机重启爬山法:  %f           %f\n", suc3/testcase, p3/suc3);
    else
        System.out.printf("随机重启爬山法:  %f           NAN\n", suc3/testcase);
    
    if (suc4!= 0)
        System.out.printf("模拟退火法:      %f           %f\n", suc4/testcase, p4/suc4);
    else
        System.out.printf("模拟退火法:      %f           NAN\n", suc4/testcase);
	}
}
