#include"object3.h"
#include"AABB3.h"
#include<algorithm>
#define STACK_SIZE 64

class BVHTree {
	struct SplitData{
		long double sah;
		int axis;
		bool subdivideNext;
		size_t primsOnLeftSide;
		size_t primsOnRightSide;
		res.leftBounds = leftBounds;
		res.rightBounds = rightBounds[i - 1];
	};

	struct BoxLessMax {
		BoxLessMax(int a) { 
			axis = a;
		}
		bool operator()(const Object3* p1, const Object3* p2) const {
			return (p1->getMax(axis) < p2->getMax(axis));
		}
		int axis;
	};

	SplitData FindObjectSplit(std::vector<Object3*>& a_plistX, std::vector<Object3*>& a_plistY, std::vector<Object3*>& a_plistZ, const AABB3 a_box) {
		

		std::vector<Object3*>* plists[3] = { &a_plistX, &a_plistY, &a_plistZ };
		BVHTree::SplitData res;
		res.sah = a_box.getArea()*a_plistX.size(); // SAH_OVERSPLIT_TRESHOLD usually equals to 1.0f
		res.subdivideNext = false;
		res.axis = -1;
		std::vector<AABB3> rightBounds(a_plistX.size());
		for (int dim = 0; dim < 3; dim++)
		{
			// sort data according to axis
			//
			std::vector<Object3*>& plist = *plists[dim];
			std::sort(plist.begin(), plist.end(), BoxLessMax(dim));
			// Sweep right to left and determine bounds.
			//
			AABB3 rightBounds1;
			for (size_t i = plist.size() - 1; i > 0; i--)
			{
				rightBounds1.include(plist[i].Box());
				rightBounds[i - 1] = rightBounds1;
			}
			// Sweep left to right and select lowest SAH.
			//
			AABB3 leftBounds;
			for (size_t i = 1; i < plist.size(); i++)
			{
				leftBounds.include(plist[i - 1].Box());
				float sah = EMPTY_NODE_COST_TRAVERSE + leftBounds.getArea()*(i)+rightBounds[i - 1].getArea() * (plist.size() - i);
				if (sah < res.sah)
				{
					res.sah = sah;
					res.axis = dim;
					res.primsOnLeftSide = i;
					res.primsOnRightSide = plist.size() - i;
					res.leftBounds = leftBounds;
					res.rightBounds = rightBounds[i - 1];
				}
			}
		} // for (int dim=0;dim<3;dim++)
		res.subdivideNext = (res.axis != -1) && (res.sah < SAH_OVERSPLIT_TRESHOLD*a_box.getArea()*a_plistX.size());
		return res;
	}

	struct TraversalStackElement
	{
		float t_far;
		int   offset;
	};

	void BVHTraversal(Point3 ray_pos, Point3 ray_dir, Lite_Hit* pClosestHit)
	{
		TraversalStackElement stack[STACK_SIZE];
		int top = 0;
		stack[0].t_far = INFINITY; // this is need
		*pClosestHit = Lite_Hit::NONE; // no intersection found
		int leftNodeOffset = 1;
		Point3 invDir = SafeInverse(ray_dir);
		while (true)
		{
			// traversal code
			//
			while (true)
			{
				BVHNode leftNode = GetBVHNode(leftNodeOffset);
				BVHNode rightNode = GetBVHNode(leftNodeOffset + 1);   // right node
				int leftRightOffsetAndLeaf = rightNode.m_leftOffsetAndLeaf;
				int leftLeftOffsetAndLeaf = leftNode.m_leftOffsetAndLeaf;
				float c0min = 0;
				float c1min = 0;
				bool traverseChild0 = RayBoxIntersection(ray_pos, invDir, leftNode.box, &c0min);
				bool traverseChild1 = RayBoxIntersection(ray_pos, invDir, rightNode.box, c1min);
				traverseChild0 = traverseChild0 && (pClosestHit->t >= c0min);
				traverseChild1 = traverseChild1 && (pClosestHit->t >= c1min);
				// traversal decision
				//
				if (traverseChild0 && traverseChild1)
				{
					// determine nearest node
					//
					stack[top].t_far = fmaxf(c1min, c0min);
					stack[top].offset = (c0min <= c1min) ? leftRightOffsetAndLeaf : leftLeftOffsetAndLeaf;
					leftNodeOffset = (c0min <= c1min) ? leftLeftOffsetAndLeaf : leftRightOffsetAndLeaf;
					top++;
				}
				else if (traverseChild0 && !traverseChild1)
					leftNodeOffset = leftLeftOffsetAndLeaf;
				else if (!traverseChild0 && traverseChild1)
					leftNodeOffset = leftRightOffsetAndLeaf;
				else // both miss, stack.pop()
				{
					if (top == 0)
						return;
					do
					{
						top--;
						leftNodeOffset = stack[top].offset;
					} while (top != 0 && pClosestHit->t < stack[top].t_far);
				}
				if (BVHNode::IS_LEAF(leftNodeOffset))
					break;
				leftNodeOffset = BVHNode::EXTRACT_OFFSET(leftNodeOffset);
			} //  end traversal code

			  // intersection code
			  //
			do
			{
				int primsListOffset = BVHNode::EXTRACT_PRIMITIVE_LIST_OFFSET(leftNodeOffset);
				IntersectAllPrimitivesInLeaf(ray_pos, ray_dir, primsListOffset, pClosestHit);
				if (top == 0)
					return;
				do
				{
					top--;
					leftNodeOffset = stack[top].offset;
				} while (top != 0 && pClosestHit->t < stack[top].t_far);
			} while (BVHNode::IS_LEAF(leftNodeOffset));
			leftNodeOffset = BVHNode::EXTRACT_OFFSET(leftNodeOffset);
		}

	}
};