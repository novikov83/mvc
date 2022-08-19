#include <iostream>
#include <vector>
#include <memory>

class IShape
{
public:
	virtual void Draw() = 0;
	virtual ~IShape() = default;
};
class TCircle: public IShape
{
public:
	TCircle(int aX, int aY, int aRadius): mX{aX}, mY{aY}, mRadius{aRadius} {};
	void Draw() override {
		std::cout << "Draw circle [x = " << mX << ", y = " << mY << ", R = " << mRadius << "]" << std::endl;
	};
private:
	int	mX;
	int	mY;
	int	mRadius;
};

class IView
{
public:
	virtual void doUpdate(const std::vector<IShape*>& aShapes) = 0;
	virtual ~IView() = default;
};
class TView: public IView
{
public:
	void doUpdate(const std::vector<IShape*>& aShapes) override {
		for (const auto& s : aShapes) {
			s->Draw();
		}
	};
};

class IModel
{
public:
	IModel(std::shared_ptr<IView> aView): mView{aView} {};
	virtual void NewDocument() = 0;
	virtual void ImportDocument(std::string aFilenName) = 0;
	virtual void ExportDocument(std::string aFilenName) = 0;
	virtual void CreateCircle(int aX, int aY, int aRadius) = 0;
	virtual ~IModel() = default;

	std::shared_ptr<IView>	mView;
};
class TModel: public IModel
{
public:
	TModel(std::shared_ptr<IView> aView): IModel{aView} {};

	void NewDocument() override {
		mShapes.clear();
		mView->doUpdate(mShapes);
	};
	void ImportDocument(std::string aFilenName) override {
		std::cout << "Load shapes from file: " << aFilenName << std::endl; 
		mView->doUpdate(mShapes);
	};
	void ExportDocument(std::string aFilenName) override {
		std::cout << "Save shapes into file: " << aFilenName << std::endl; 
	};
	void CreateCircle(int aX, int aY, int aRadius) override {
		TCircle*	circle = new TCircle{aX, aY, aRadius};
		mShapes.emplace_back(circle);
		mView->doUpdate(mShapes);
	};

private:
	std::vector<IShape*>	mShapes;
};

class TController
{
public:
	TController(std::shared_ptr<IModel> aModel) : mModel{aModel} {};

	void NewDocument() {
		mModel->NewDocument();
	}
	void ImportDocument(std::string aFilenName) {
		mModel->ImportDocument(aFilenName);
	}
	void ExportDocument(std::string aFilenName) {
		mModel->ExportDocument(aFilenName);
	}
	void CreateCircle(int aX, int aY, int aRadius) {
		mModel->CreateCircle(aX, aY, aRadius);
	}

private:
	std::shared_ptr<IModel> mModel;
};


int main(int, char**)
{
	std::shared_ptr<TView>			View(new TView);
	std::shared_ptr<TModel>			Model(new TModel{View});
	std::shared_ptr<TController>	Controller(new TController{Model});

	Controller->NewDocument();
	Controller->CreateCircle(4, 5, 6);
	Controller->ExportDocument("FileForExport.editor");
	Controller->ImportDocument("FileForImport.editor");
	return 0;
}
