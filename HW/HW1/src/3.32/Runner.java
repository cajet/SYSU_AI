import java.io.IOException;
import jigsaw.Jigsaw;
import jigsaw.JigsawNode;

public class Runner {
	/**利用启发式搜索，求解随机5*5拼图（24-数码问题）
	 * 注意：运行前要修改节点维数，将JigsawNode类中的dimension改为5
	 * 要求：不修改脚本内容，程序能够运行，且得出预期结果
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws IOException {
		// 检查节点维数是否为5
		if(JigsawNode.getDimension() != 3){
			System.out.print("节点维数不正确，请将JigsawNode类的维数dimension改为3");
			return;
		}
		
		// 生成目标状态对象destNode: {9,1,2,3,4,5,6,7,8,0}
		JigsawNode destNode = new JigsawNode(new int[]{9,1,2,3,4,5,6,7,8,0});	

		// 执行启发式搜索示例算法， 100个测试样例
		int testcase = 100;
		double t1, t2, t3, t4;
		t1 = t2 = t3 = t4 = 0; //标记4个函数搜索成功的样例个数
		double num1, num2, num3, num4;
		num1 = num2 = num3 = num4 = 0;
		for (int i = 0; i < testcase; i++) {
			System.out.println("正在进行第"+i+"个测试");
			JigsawNode startNode = Jigsaw.scatter(destNode, 1000);
			Jigsaw jigsaw = new Jigsaw(startNode, destNode);
			//错位数码个数
			if (jigsaw.ASearch(3)) {
				num3 += jigsaw.getSearchedNodesNum();
				t3++;
			}
			//后续节点不正确的数码个数
			if (jigsaw.ASearch(4)) {
				num4 += jigsaw.getSearchedNodesNum();
				t4++;
			}
			//曼哈顿距离
			if (jigsaw.ASearch(1)) {
				num1 += jigsaw.getSearchedNodesNum();
				t1++;
			}
			//几何距离
			if (jigsaw.ASearch(2)) {
				num2 += jigsaw.getSearchedNodesNum();
				t2++;
			}
		}

		System.out.println("完成100个测试样例，性能分析如下:\n");
		System.out.println("                 耗散          解决率");
		System.out.printf("曼哈顿距离     %f            %f\n", num1/t1, t1/testcase);
		System.out.printf("几何距离       %f            %f\n", num2/t2, t2/testcase);
		System.out.printf("错位码个数     %f            %f\n", num3/t3, t3/testcase);
		System.out.printf("后续错位码数   %f           %f\n", num4/t4, t4/testcase);

	}

}
