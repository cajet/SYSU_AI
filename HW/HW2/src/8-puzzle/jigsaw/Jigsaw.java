package jigsaw;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Iterator;
import java.util.Random;
import java.util.Vector;
import java.lang.Math;

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
			if(tempJNode.move(i) && !this.closeList.contains(tempJNode) && !this.openList.contains(tempJNode)) {
				this.estimateValue(tempJNode);
				followJNodes.addElement(tempJNode);
			}
		}
		return followJNodes;
	}

	/**排序插入openList：按照节点的代价估值（estimatedValue）将节点插入openList中，估值小的靠前。
	 * @param jNode - 要插入的状态节点
	 */
	private void sortedInsertOpenList(JigsawNode jNode) {
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
	
	/**最陡上升爬山算法：求解指定3*3拼图（8-数码问题）的最优解。*/
	public boolean M_HillClimb_Search() throws IOException {
		// 访问节点数过大则认为搜索失败
		int maxNodesNum = 30000;  
		
		// 用以存放某一节点的邻接节点
		Vector<JigsawNode> followJNodes = new Vector<JigsawNode>(); 
		
		// 重置求解完成标记为false
		this.openList = new Vector<JigsawNode>();
		this.closeList = new Vector<JigsawNode>();
		this.isCompleted = false;
		this.searchedNodesNum = 0;        
		
		// 将起始节点放入openList中
		this.estimateValue(this.beginJNode);
		this.openList.addElement(this.beginJNode);
		
		// 如果openList为空，或者访问节点数大于maxNodesNum个，则搜索失败，问题无解;否则循环直到求解成功
		while (this.openList.isEmpty() != true && searchedNodesNum <= maxNodesNum) {
			
			// 若currentJNode为目标节点，则搜索成功，设置完成标记isCompleted为true，计算解路径，退出。
			this.currentJNode = this.openList.elementAt(0);
			if (this.currentJNode.equals(this.endJNode)){
				isCompleted = true;
				break;
			}
			
			// 从openList中删除节点N,并将其放入closeList中，表示以访问节点			
			this.openList.removeElementAt(0);
			this.closeList.addElement(this.currentJNode);
			searchedNodesNum++;
			
			// 寻找最优邻接节点
			followJNodes = this.findFollowJNodes(this.currentJNode);
			JigsawNode tempJNode = followJNodes.elementAt(0);
			for (int i = 1; i < followJNodes.size(); i++) {
				if (followJNodes.elementAt(i).getEstimatedValue() < tempJNode.getEstimatedValue())
					tempJNode = followJNodes.elementAt(i);
			}
			//若最优后继节点优于当前节点，则加入探索队列，否则将会结束探索
			if (tempJNode.getEstimatedValue() < this.currentJNode.getEstimatedValue())
				this.openList.addElement(tempJNode);
		}
		return isCompleted;
	}
	
	/**首选爬山算法：求解指定3*3拼图（8-数码问题）的最优解。*/
	public boolean F_HillClimb_Search() throws IOException{
		// 访问节点数大于25000个则认为搜索失败
		int maxNodesNum = 25000;  
		
		// 用以存放某一节点的邻接节点
		Vector<JigsawNode> followJNodes = new Vector<JigsawNode>(); 
		
		// 重置求解完成标记为false
		this.openList = new Vector<JigsawNode>();
		this.closeList = new Vector<JigsawNode>();
		this.isCompleted = false;
		this.searchedNodesNum = 0;        
		
		// 将起始节点放入openList中
		this.estimateValue(this.beginJNode);
		this.openList.addElement(this.beginJNode);
		
		// 如果openList为空，或者访问节点数大于maxNodesNum个，则搜索失败，问题无解;否则循环直到求解成功
		while (this.openList.isEmpty() != true && searchedNodesNum <= maxNodesNum) {
			
			// 若currentJNode为目标节点，则搜索成功，设置完成标记isCompleted为true，计算解路径，退出。
			this.currentJNode = this.openList.elementAt(0);
			if (this.currentJNode.equals(this.endJNode)){
				isCompleted = true;
				break;
			}
			
			// 从openList中删除节点N,并将其放入closeList中，表示以访问节点			
			this.openList.removeElementAt(0);
			this.closeList.addElement(this.currentJNode);
			searchedNodesNum++;

			// 随机生成currentJNode的后继节点，直到找到一个优于当前节点的后继节点邻接，插入openList中
			followJNodes = this.findFollowJNodes(this.currentJNode);
			while (!followJNodes.isEmpty()) {
				if (followJNodes.elementAt(0).getEstimatedValue() < this.currentJNode.getEstimatedValue()) {
					openList.add(followJNodes.elementAt(0));
					break;
				}
				followJNodes.removeElementAt(0);
			}
		}
		return isCompleted;
	}

	/**随机重启爬山算法：求解指定3*3拼图（8-数码问题）的最优解。*/
	public boolean R_HillClimb_Search() throws IOException{
		// 访问节点数大于25000个则认为搜索失败
		int maxNodesNum = 25000;  
		
		// 用以存放某一节点的邻接节点
		Vector<JigsawNode> followJNodes = new Vector<JigsawNode>(); 
		
		// 重置求解完成标记为false
		this.openList = new Vector<JigsawNode>();
		this.closeList = new Vector<JigsawNode>();
		this.isCompleted = false;
		this.searchedNodesNum = 0;           
		
		// 将起始节点放入openList中
		this.estimateValue(this.beginJNode);
		this.openList.addElement(this.beginJNode);
		
		// 如果openList为空，或者该路径访问节点数大于maxNodesNum个，则搜索失败，问题无解;否则循环直到求解成功
		while (!isCompleted) {
			if (this.openList.isEmpty() || searchedNodesNum%(maxNodesNum+1) >= maxNodesNum) {
				this.beginJNode = Jigsaw.scatter(this.endJNode, 1000);
		    	this.openList = new Vector<JigsawNode>();
				this.closeList = new Vector<JigsawNode>();
				followJNodes = new Vector<JigsawNode>(); 
				this.sortedInsertOpenList(this.beginJNode);
			}
			// 访问openList的第一个节点N，置为当前节点currentJNode
			// 若currentJNode为目标节点，则搜索成功，设置完成标记isCompleted为true，计算解路径，退出。
			this.currentJNode = this.openList.elementAt(0);
			if (this.currentJNode.equals(this.endJNode)){
				isCompleted = true;
				break;
			}
			
			// 从openList中删除节点N,并将其放入closeList中，表示以访问节点			
			this.openList.removeElementAt(0);
			this.closeList.addElement(this.currentJNode);
			searchedNodesNum++;
			
			// 随机生成currentJNode的后继节点，直到找到一个优于当前节点的后继节点邻接，插入openList中
			followJNodes = this.findFollowJNodes(this.currentJNode);
			while (!followJNodes.isEmpty()) {
				if (followJNodes.elementAt(0).getEstimatedValue() < this.currentJNode.getEstimatedValue()) {
					openList.add(followJNodes.elementAt(0));
					break;
				}
				followJNodes.removeElementAt(0);
			}
		}
		return isCompleted;
	}

	/**模拟退火算法：求解指定3*3拼图（8-数码问题）的最优解。*/
	public boolean S_annealing_Search() throws IOException {
		// 用以存放某一节点的邻接节点
		Vector<JigsawNode> followJNodes = new Vector<JigsawNode>(); 
		
		// 重置求解完成标记为false
		this.openList = new Vector<JigsawNode>();
		this.closeList = new Vector<JigsawNode>();
		this.isCompleted = false;
		this.searchedNodesNum = 0;           
		
		// 将起始节点放入openList中
		this.estimateValue(this.beginJNode);
		this.openList.addElement(this.beginJNode);

		double T;        //温度
		double cold = 0.95;   //冷却因子
		double e = 2.71828;   //概率计算的底数

		// 当后继节点不为空，根据温度和时间的函数来进行搜索
		while (!this.openList.isEmpty()) {
			this.currentJNode = this.openList.elementAt(0);
			if (this.currentJNode.equals(this.endJNode)){
				isCompleted = true;
				break;
			}
			
			// 从openList中删除节点N,并将其放入closeList中，表示以访问节点			
			this.openList.removeElementAt(0);
			this.closeList.addElement(this.currentJNode);
			searchedNodesNum++;
			
			// 随机生成下一个邻接点，当邻接点差于当前节点，按照时间温度函数来决定是否前进
			followJNodes = this.findFollowJNodes(this.currentJNode);
			T = 300;
			while (!followJNodes.isEmpty()) {
				double delta = (double)(this.currentJNode.getEstimatedValue())-(double)(followJNodes.elementAt(0).getEstimatedValue());
				T *= cold;  //降温
				if (delta > 0) {
					openList.add(followJNodes.elementAt(0));
					break;
				} else {
					double p = Math.random();
					//命中概率，则添加该邻接点
					if (p < Math.pow(e, delta/T)) {
						openList.add(followJNodes.elementAt(0));
						break;
					}
				}
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
		int f1 = 0; //所有 放错位的数码 个数
		int f2 = 0; //所有 放错位的数码与其正确位置的距离之和 曼哈顿距离
		int f3 = 0; //几何距离
		int dimension = JigsawNode.getDimension();
		for(int index =1 ; index< dimension*dimension; index++){
			if (jNode.getNodesState()[index] != index && jNode.getNodesState()[0] != index) {
				f1++;
				int x1 = (index-1) / dimension;
				int y1 = (index-1) % dimension;
				int x2 = (jNode.getNodesState()[index]-1) / dimension;
				int y2 = (jNode.getNodesState()[index]-1) % dimension;
				f2 += Math.abs(x1-x2) + Math.abs(y1-y2);
				f3 += Math.sqrt(Math.abs(x1-x2)) + Math.sqrt(Math.abs(y1-y2));
			}
		}
		jNode.setEstimatedValue(1*f1 + 6*f2 + 2*f3);
	}

}
