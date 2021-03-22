#include "bvh_tree.h"

BVHTree::BVHTree(std::vector<Triangle *>& _triangles)
{
	std::srand(std::time(NULL));
	std::vector<AABB> aabbList;

	for (auto& triangle : _triangles) {
		aabbList.push_back(AABB(triangle));
	}

	std::sort(aabbList.begin(), aabbList.end(), BVHNode::sortByAxisX);

	root = new BVHNode();

	RecursiveBuildup(aabbList, root);
}

BVHTree::BVHTree() : root(nullptr)
{

}

void BVHTree::RecursiveBuildup(std::vector<AABB>& _boxes, BVHNode* _parent)
{
	int axis = int(std::rand() * 30 % 3);
	float mid = 0;
	if (axis == 0) {
		std::sort(_boxes.begin(), _boxes.end(), BVHNode::sortByAxisX);
	}
	if (axis == 1) {
		std::sort(_boxes.begin(), _boxes.end(), BVHNode::sortByAxisY);
	}
	if (axis == 2) {
		std::sort(_boxes.begin(), _boxes.end(), BVHNode::sortByAxisZ);
	}


	AABB containBox(_boxes);
	_parent->box = containBox;
	mid = containBox.Midpoint(axis);


	if (_boxes.size() == 1) {
		_parent->leaf = true;
		_parent->box.triangle = _boxes[0].triangle;
	}
	else if (_boxes.size() == 2) {
		BVHNode* left = new BVHNode();
		BVHNode* right = new BVHNode();

		_parent->left = left;
		_parent->right = right;

		std::vector<AABB> leftAABB;
		std::vector<AABB> rightAABB;
		leftAABB.push_back(_boxes[0]);
		rightAABB.push_back(_boxes[1]);

		RecursiveBuildup(leftAABB, left);
		RecursiveBuildup(rightAABB, right);
	}
	else {
		BVHNode* left = new BVHNode();
		BVHNode* right = new BVHNode();

		_parent->left = left;
		_parent->right = right;

		std::vector<AABB> leftAABB;
		std::vector<AABB> rightAABB;
		for (auto& aabb : _boxes) {
			if (aabb.Midpoint(axis) < mid) {
				leftAABB.push_back(aabb);
			}
			else {
				rightAABB.push_back(aabb);
			}
		}

		if (leftAABB.size() == 0) {
			leftAABB.push_back(rightAABB[0]);
			rightAABB.erase(rightAABB.begin(), rightAABB.begin() + 1);
		}
		if (rightAABB.size() == 0) {
			rightAABB.push_back(leftAABB[0]);
			leftAABB.erase(leftAABB.begin(), leftAABB.begin() + 1);
		}

		RecursiveBuildup(leftAABB, left);
		RecursiveBuildup(rightAABB, right);
	}
}

bool BVHTree::Hit(Ray& _ray, HitResult& _hit, bool _culling, float _minimum)
{
	return root->Hit(_ray, _hit, _minimum, _culling);
}
