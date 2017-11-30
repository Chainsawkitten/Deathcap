namespace Animation {
	struct SkeletonBone {
		glm::mat4 localTx;
		glm::mat4 globalTx;
		glm::mat4 inversed;
		uint32_t parentId;
		
		/// Save skeleton bone.
		/**
		 * @param file File to save to.
		 */
		void Save(std::ofstream* file) {
			file->write(reinterpret_cast<char*>(&localTx), sizeof(float) * 16);
			file->write(reinterpret_cast<char*>(&globalTx), sizeof(float) * 16);
			file->write(reinterpret_cast<char*>(&inversed), sizeof(float) * 16);
			file->write(reinterpret_cast<char*>(&parentId), sizeof(uint32_t));
		}
	
		/// Load skeleton bone.
		/**
		 * @param file File to load from.
		 */
		void Load(std::ifstream* file) {
			file->read(reinterpret_cast<char*>(&localTx), sizeof(float) * 16);
			file->read(reinterpret_cast<char*>(&globalTx), sizeof(float) * 16);
			file->read(reinterpret_cast<char*>(&inversed), sizeof(float) * 16);
			file->read(reinterpret_cast<char*>(&parentId), sizeof(uint32_t));
		}
	};
}