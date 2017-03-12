package jigsaw;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Iterator;
import java.util.Vector;

/** 在此类中填充算法，完成重拼图游戏（N-数码问题）
 * @author abe
 *
 */
public class Jigsaw {
	JigsawNode beginJNode;		// 拼图的起始状态节点
	JigsawNode endJNode;		// 拼图的目标状态节点
	JigsawNode currentJNode;	// 拼图的当前状态节点
	private Vector<JigsawNode> openList;	// open表 ：用以保存已发现但未访问的节点
	private Vector<JigsawNode> closeList;	// close表：用以保存已访问的节点
	private Vector<JigsawNode> solutionPath;// 解路径  ：用以保存从起始状态到达目标状态的移动路径中的每一个状态节点
	private boolean isCompleted;	// 完成标记：初始为false;当求解成功时，将该标记至为true
	private int searchedNodesNum;	// 已访问节点数： 用以记录所有访问过的节点的数量
	private int type;               //记录启发式函数的类型

	/**拼图构造函数
	 * @param bNode - 初始状态节点
	 * @param eNode - 目标状态节点
	 */
	public Jigsaw(JigsawNode bNode, JigsawNode eNode) {
		this.beginJNode = new JigsawNode(bNode);
		this.endJNode = new JigsawNode(eNode);
		this.currentJNode = new JigsawNode(bNode);
		this.openList = new Vector<JigsawNode>();
		this.closeList = new Vector<JigsawNode>();
		this.solutionPath = null;
		this.isCompleted = false;
		this.searchedNodesNum = 0;
	}

	/**此函数用于打散拼图：将输入的初始状态节点jNode随机移动len步，返回其打散后的状态节点
	 * @param jNode - 初始状态节点
	 * @param len - 随机移动的步数
	 * @return 打散后的状态节点
	 */
	public static JigsawNode scatter(JigsawNode jNode, int len) {
		int randomDirection;
		len += (int) (Math.random() * 2);
		JigsawNode jigsawNode = new JigsawNode(jNode);
		for (int t = 0; t < len; t++) {
			int[] movable = jigsawNode.canMove();
			do{randomDirection = (int) (Math.random() * 4);}
			while(0 == movable[randomDirection]);
			jigsawNode.move(randomDirection);
		}
		jigsawNode.setInitial();
		return jigsawNode;
	}

	/**获取拼图的当前状态节点
	 * @return currentJNode -  拼图的当前状态节点
	 */
	public JigsawNode getCurrentJNode() {
		return currentJNode;
	}

	/**设置拼图的初始状态节点
	 * @param jNode - 拼图的初始状态节点
	 */
	public void setBeginJNode(JigsawNode jNode) {
		beginJNode = jNode;
	}

	/**获取拼图的初始状态节点
	 * @return beginJNode - 拼图的初始状态节点
	 */
	public JigsawNode getBeginJNode() {
		return beginJNode;
	}

	/**设置拼图的目标状态节点
	 * @param jNode - 拼图的目标状态节点
	 */
	public void setEndJNode(JigsawNode jNode) {
		this.endJNode = jNode;
	}

	/**获取拼图的目标状态节点
	 * @return endJNode - 拼图的目标状态节点
	 */
	public JigsawNode getEndJNode() {
		return endJNode;
	}

	/**获取拼图的求解状态
	 * @return isCompleted - 拼图已解为true；拼图未解为false
	 */
	public boolean isCompleted() {
		return isCompleted;
	}

	/**获取访问过的节点数searchedNodesNum
	 * @return 返回所有已访问过的节点总数
	 */
	public int getSearchedNodesNum() {
		return searchedNodesNum;
	}
	

	/**探索所有与jNode邻接(上、下、左、右)且未曾被访问的节点
	 * @param jNode - 要探索的节点
	 * @return 包含所有与jNode邻接且未曾被访问的节点的Vector<JigsawNode>对象
	 */
	private Vector<JigsawNode> findFollowJNodes(JigsawNode jNode) {
		Vector<JigsawNode> followJNodes = new Vector<JigsawNode>();
		JigsawNode tempJNode;
		for(int i=0; i<4; i++){
			tempJNode = new JigsawNode(jNode);
			if(tempJNode.move(i) && !this.closeList.contains(tempJNode) && !this.openList.contains(tempJNode))
				followJNodes.addElement(tempJNode);
		}
		return followJNodes;
	}

	/**排序插入openList：按照节点的代价估值（estimatedValue）将节点插入openList中，估值小的靠前。
	 * @param jNode - 要插入的状态节点
	 */
	private void sortedInsertOpenList(JigsawNode jNode) {
		this.estimateValue(jNode);
		for (int i = 0; i < this.openList.size(); i++) {
			if (jNode.getEstimatedValue() < this.openList.elementAt(i)
					.getEstimatedValue()) {
				this.openList.insertElementAt(jNode, i);
				return;
			}
		}
		this.openList.addElement(jNode);
	}
	
	
	
	// ****************************************************************
	/**启发式搜索算法，求解随机3*3拼图（8-数码问题）
	 * 要求：1.修改启发式搜索算法ASearch()和代价估计函数estimateValue()
	 *      2.访问节点总数不超过25000个
	 * 主要涉及函数：ASearch()，estimateValue()
	 */
	// ****************************************************************
	public boolean ASearch(int n) throws IOException{
		this.type = n;
		
		// 访问节点数大于30000个则认为搜索失败0s
		int maxNodesNum = 30000;  
		
		// 用以存放某一节点的邻接节点
		Vector<JigsawNode> followJNodes = new Vector<JigsawNode>(); 
		
		// 重置求解完成标记为false
		this.openList = new Vector<JigsawNode>();
		this.closeList = new Vector<JigsawNode>();
		this.isCompleted = false;
		this.searchedNodesNum = 0;           
		
		// (1)将起始节点放入openList中
		this.sortedInsertOpenList(this.beginJNode);
		
		// (2) 如果openList为空，或者访问节点数大于maxNodesNum个，则搜索失败，问题无解;否则循环直到求解成功
		while (this.openList.isEmpty() != true && searchedNodesNum <= maxNodesNum) {
			
			// (2-1)访问openList的第一个节点N，置为当前节点currentJNode
			//      若currentJNode为目标节点，则搜索成功，设置完成标记isCompleted为true，计算解路径，退出。
			this.currentJNode = this.openList.elementAt(0);
			if (this.currentJNode.equals(this.endJNode)){
				isCompleted = true;
				break;
			}
			
			// (2-2)从openList中删除节点N,并将其放入closeList中，表示以访问节点			
			this.openList.removeElementAt(0);
			this.closeList.addElement(this.currentJNode);
			searchedNodesNum++;
			
			// (2-3)寻找所有与currentJNode邻接且未曾被访问的节点，将它们按代价估值从小到大排序插入openList中
			followJNodes = this.findFollowJNodes(this.currentJNode);
			while (!followJNodes.isEmpty()) {
				this.sortedInsertOpenList(followJNodes.elementAt(0));
				followJNodes.removeElementAt(0);
			}
		}
		return isCompleted;
	}
	
	/**计算并修改状态节点jNode的代价估计值:f(n)=s(n)。
	 * s(n)代表后续节点不正确的数码个数
	 * @param jNode - 要计算代价估计值的节点；此函数会改变该节点的estimatedValue属性值。
	 */
	private void estimateValue(JigsawNode jNode) {
		int f1 = 0; //后续节点不正确的数码个数
		int f2 = 0; //所有放错位的数码 个数
		int f3 = 0; //所有放错位的数码与其正确位置的距离之和 曼哈顿距离
		int f4 = 0; //所有放错位的数码与其正确位置的距离之和 几何距离
		int dimension = JigsawNode.getDimension();
		for(int index =1 ; index< dimension*dimension; index++){
			if(jNode.getNodesState()[index]+1!=jNode.getNodesState()[index+1])
				f1++;
			if (jNode.getNodesState()[index] != index && jNode.getNodesState()[0] != index) {
				f2++;
				int x1 = (index-1) / dimension;  //横坐标
				int y1 = (index-1) % dimension;  //纵坐标
				int x2 = (jNode.getNodesState()[index]-1) / dimension;
				int y2 = (jNode.getNodesState()[index]-1) % dimension;
				f3 += Math.abs(x1-x2) + Math.abs(y1-y2);
				f4 += Math.sqrt(Math.abs(x1-x2)) + Math.sqrt(Math.abs(y1-y2));
			}
		}

		switch (this.type) {
			case 1:
				jNode.setEstimatedValue(f3);
				break;
			case 2:
				jNode.setEstimatedValue(f4);
				break;
			case 3:
				jNode.setEstimatedValue(f2);
				break;
			default:
				jNode.setEstimatedValue(f1);
				break;
		}
	}
}
