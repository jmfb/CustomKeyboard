#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Element
{
public:
	virtual bool IsKey() const { return false; }
	virtual bool IsPlate() const { return false; }
	virtual double KeyPadding() const { return 0; }
	virtual double Width() const = 0;
};

class Spacer : public Element
{
public:
	Spacer(double mm)
		: mm(mm)
	{
	}
	bool IsPlate() const final { return true; }
	double Width() const final { return mm; }
private:
	double mm;
};

class Padding : public Element
{
public:
	Padding()
	{
	}
	double Width() const final { return 0.5; }
};

class Key : public Element
{
public:
	Key(double ratio)
		: ratio(ratio)
	{
	}
	bool IsKey() const final { return true; }
	double KeyPadding() const final { return (Width() - 14) / 2; }
	double Width() const final { return ratio * 19; }
private:
	double ratio;
};

class Row
{
public:
	Row(string name, vector<shared_ptr<Element>> elements)
		: name(name), elements(elements)
	{
	}
	double PrintAndReturnWidth() const
	{
		auto left = 0.0;
		for (auto element : elements)
		{
			if (element->IsKey())
				cout << left + element->KeyPadding() << ", ";
			left += element->Width();
		}
		return left;
	}
	void PrintKeyLeftsAndRowWidth() const
	{
		cout << name << " = [";
		auto width = PrintAndReturnWidth();
		cout << "], width = " << width << '\n';
	}
	void PrintFacePlate() const
	{
		cout << "Face Plate " << name << " = ";
		double plate = 0, space = 0;
		auto isPlate = true;
		for (auto element : elements)
		{
			if (element->IsPlate() != isPlate)
			{
				if (isPlate && plate > 0)
					cout << "[" << plate << "] ";
				else if (!isPlate && space > 0)
					cout << "<" << space << "> ";
				plate = space = 0;
				isPlate = element->IsPlate();
			}

			if (element->IsPlate())
				plate += element->Width();
			else
				space += element->Width();
		}
		if (plate > 0)
			cout << "[" << plate << "]\n";
		else if (space > 0)
			cout << "<" << space << ">\n";
	}
private:
	string name;
	vector<shared_ptr<Element>> elements;
};

template <typename... T>
inline Row MakeRow(string name, T... elements)
{
	return { name, { make_shared<T>(elements)... } };
}

const Spacer outerEdge{ 7 };
const Spacer innerEdge{ 4 };
const Padding padding;

template <typename... T>
inline void Board(string name, T... boardRows)
{
	vector<Row> rows{ boardRows... };
	cout << "Board " << name << '\n';
	for (auto& row : rows)
		row.PrintKeyLeftsAndRowWidth();
	for (auto& row : rows)
		row.PrintFacePlate();
	cout << '\n';
}

int main()
{
	Board("Left Hand",
		MakeRow("Function Row",
			outerEdge, padding,
			Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 },
			padding,
			Spacer{ Key{ 1 }.Width() - padding.Width() * 2 },
			padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding,
			Spacer{ Key{ 1.5 }.Width() - Key{ 1 }.Width() - padding.Width() * 2 },
			padding,
			Key{ 1 },
			padding, outerEdge),
		MakeRow("Row 1",
			outerEdge, padding,
			Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1.5 },
			padding, outerEdge),
		MakeRow("Row 2",
			outerEdge, padding,
			Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1.5 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 2 },
			padding, outerEdge),
		MakeRow("Row 3",
			outerEdge, padding,
			Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, Spacer{ innerEdge.Width() * 2 + Key{ 1 }.Width() * 3 + padding.Width() * 2 }, padding,
			Key{ 1.75 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1.75 },
			padding, outerEdge),
		MakeRow("Row 4",
			outerEdge, padding,
			Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, Spacer{ Key{ 1 }.Width() }, padding,
			Key{ 1 },
			padding, Spacer{ Key{ 1 }.Width() }, innerEdge, padding,
			Key{ 2.25 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1.25 },
			padding, outerEdge),
		MakeRow("Row 5",
			outerEdge, padding,
			Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1.25 }, Key{ 1.25 }, Key{ 1.25 }, Key{ 1.5 }, Key{ 2.25 }, Key{ 1 },
			padding, outerEdge)
	);

	Board("Right Hand",
		MakeRow("Function Row",
			outerEdge, padding,
			Key{ 1 }, Key{ 1 },
			padding, Spacer{ Key{ 1 }.Width() - padding.Width() * 2 }, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 },
			padding, outerEdge),
		MakeRow("Row 1",
			outerEdge, padding,
			Key{ 2 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 2 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 },
			padding, outerEdge),
		MakeRow("Row 2",
			outerEdge, padding,
			Key{ 1.5 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1.5 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 },
			padding, outerEdge),
		MakeRow("Row 3",
			outerEdge, padding,
			Key{ 1.75 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 2.25 },
			padding, Spacer{ innerEdge.Width() * 2 + Key{ 1 }.Width() * 3 + padding.Width() * 2 }, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 },
			padding, outerEdge),
		MakeRow("Row 4",
			outerEdge, padding,
			Key{ 2.25 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 2.75 },
			padding, innerEdge, Spacer{ Key{ 1 }.Width() }, padding,
			Key{ 1 },
			padding, Spacer{ Key{ 1 }.Width() }, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 },
			padding, outerEdge),
		MakeRow("Row 5",
			outerEdge, padding,
			Key{ 1 }, Key{ 2.25 }, Key{ 1.75 }, Key{ 1.25 }, Key{ 1.25 }, Key{ 1.25 }, Key{ 1.25 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 }, Key{ 1 }, Key{ 1 }, Key{ 1 },
			padding, innerEdge, padding,
			Key{ 1 },
			padding, outerEdge)
	);

	return 0;
}
