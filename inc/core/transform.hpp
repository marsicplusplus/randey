#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

class Transform {
	public:
		Transform();
		static Transform lerp(const Transform *prev, const Transform *next, float dt);

		glm::mat4 getMatrix();
		glm::mat4 getTransposeInverse();
		glm::mat4 getInverse();

		void scale(float uniform);
		void scale(glm::vec3 &scaling);
		void scale(float x, float y, float z);

		void rotate(float angle, const glm::vec3 &rotAxis);

		void translate(float x, float y, float z);
		void translate(glm::vec3 &tran);

		inline const glm::vec3& getTranslation() const { return t; }
		inline const glm::vec3& getScale() const { return s; }
		inline const glm::quat& getRotation() const { return r; }

	private:
		glm::vec3 s;
		glm::vec3 t;
		glm::quat r;
		glm::mat4 m;
		glm::mat4 mInv;
		glm::mat4 mInvTran;

        bool mDirty = true;

		void updateMatrix();
};

#endif