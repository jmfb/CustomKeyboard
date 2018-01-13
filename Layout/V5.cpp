#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std;

const double tops[] = { 10, 38, 57, 76, 95, 114 };
const double dsubSpace = 12;
const double keyHole = 14;

class Element
{
public:
	virtual bool IsKey() const { return false; }
	virtual bool IsPlate() const { return false; }
	virtual double KeyPadding() const { return 0; }
	virtual double Width() const = 0;
	virtual string Name() const { return ""; }
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
	Key(double ratio, string name = "")
		: ratio(ratio), name(name)
	{
	}
	bool IsKey() const final { return true; }
	double KeyPadding() const final { return (Width() - 14) / 2; }
	double Width() const final { return ratio * 19; }
	string Name() const final { return name; }
private:
	double ratio;
	string name;
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
	void PrintPcbPositions(double top) const
	{
		cout << name << ", top = " << top << '\n';
		auto width = PrintPcbPositionsAndReturnWidth();
		cout << "(width = " << width << ")\n\n";
	}
	double PrintPcbPositionsAndReturnWidth() const
	{
		auto left = 0.0;
		for (auto element : elements)
		{
			if (element->IsKey())
				cout << "  " << element->Name() << " = " << (left + element->KeyPadding() + (keyHole / 2)) << '\n';
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
const Spacer dsubEdge{ dsubSpace };

template <typename... T>
inline void Board(string name, T... boardRows)
{
	vector<Row> rows{ boardRows... };
	cout << "Board " << name << '\n';
	auto rowIndex = 0;
	// for (auto& row : rows)
	// 	row.PrintPcbPositions(tops[rowIndex++] + dsubSpace + (keyHole / 2));
	// for (auto& row : rows)
	// 	row.PrintKeyLeftsAndRowWidth();
	for (auto& row : rows)
		row.PrintFacePlate();
	cout << '\n';
}

int main()
{
	Board("Left Hand",
		MakeRow("Function Row",
			dsubEdge,
			outerEdge, padding,
			Key{ 1, "ESC" },
			padding,
			Spacer{ Key{ 1 }.Width() - padding.Width() * 2 },
			padding,
			Key{ 1, "F1" }, Key{ 1, "F2" }, Key{ 1, "F3" }, Key{ 1, "F4" }, Key{ 1, "F5" },
			padding,
			Spacer{ Key{ 0.5 }.Width() },
			outerEdge),
		MakeRow("Row 1",
			dsubEdge,
			outerEdge, padding,
			Key{ 1, "BACKTICK" }, Key{ 1, "1" }, Key{ 1, "2" }, Key{ 1, "3" }, Key{ 1, "4" }, Key{ 1, "5" }, Key{ 1, "6" },
			padding,
			Spacer{ Key{ 0.5 }.Width() },
			outerEdge),
		MakeRow("Row 2",
			dsubEdge,
			outerEdge, padding,
			Key{ 1.5, "TAB" }, Key{ 1, "Q" }, Key{ 1, "W" }, Key{ 1, "E" }, Key{ 1, "R" }, Key{ 1, "T" },
			padding,
			Spacer{ Key{ 1 }.Width() },
			outerEdge),
		MakeRow("Row 3",
			dsubEdge,
			outerEdge,
			Spacer{ Key{ 1.75 }.Width() },
			padding,
			Key{ 1, "A" }, Key{ 1, "S" }, Key{ 1, "D" }, Key{ 1, "F" }, Key{ 1, "G" },
			padding,
			Spacer{ Key{ 0.75 }.Width() },
			outerEdge),
		MakeRow("Row 4",
			dsubEdge,
			outerEdge, padding,
			Key{ 2.25, "LEFT_SHIFT" }, Key{ 1, "Z" }, Key{ 1, "X" }, Key{ 1, "C" }, Key{ 1, "V" }, Key{ 1, "B" },
			padding,
			Spacer{ Key{ 0.25 }.Width() },
			outerEdge),
		MakeRow("Row 5",
			dsubEdge,
			outerEdge, padding,
			Key{ 1.25, "LEFT_CTRL" }, Key{ 1.25, "LEFT_WIN" }, Key{ 1.25, "LEFT_ALT" }, Key{ 1.5, "LEFT_HYPER" }, Key{ 2.25, "LEFT_SPACE" },
			padding, outerEdge)
	);

	Board("Right Hand",
		MakeRow("Function Row",
			outerEdge,
			Spacer{ Key{ 2 }.Width() },
			padding,
			Key{ 1, "F6" }, Key{ 1, "F7" }, Key{ 1, "F8" }, Key{ 1, "F9" }, Key{ 1, "F10" }, Key{ 1, "F11" }, Key{ 1, "F12" },
			padding, innerEdge, padding,
			Key{ 1, "PRINT" }, Key{ 1, "SCROLL" }, Key{ 1, "PAUSE" },
			padding, innerEdge, padding,
			Key{ 1, "N1" }, Key{ 1, "N2" }, Key{ 1, "N3" }, Key{ 1, "N4" },
			padding, outerEdge,
			dsubEdge),
		MakeRow("Row 1",
			outerEdge,
			Spacer{ Key{ 1 }.Width() },
			padding,
			Key{ 1, "7" }, Key{ 1, "8" }, Key{ 1, "9" }, Key{ 1, "0" }, Key{ 1, "MINUS" }, Key{ 1, "EQUALS" }, Key{ 2, "BACKSPACE" },
			padding, innerEdge, padding,
			Key{ 1, "INSERT" }, Key{ 1, "HOME" }, Key{ 1, "PAGE_UP" },
			padding, innerEdge, padding,
			Key{ 1, "NUM_LOCK" }, Key{ 1, "NUM_DIV" }, Key{ 1, "NUM_MUL" }, Key{ 1, "NUM_MINUS" },
			padding, outerEdge,
			dsubEdge),
		MakeRow("Row 2",
			outerEdge,
			Spacer{ Key{ 0.5 }.Width() },
			padding,
			Key{ 1, "Y" }, Key{ 1, "U" }, Key{ 1, "I" }, Key{ 1, "O" }, Key{ 1, "P" }, Key{ 1, "LEFT_BRACE" }, Key{ 1, "RIGHT_BRACE" }, Key{ 1.5, "BACKSLASH" },
			padding, innerEdge, padding,
			Key{ 1, "DELETE" }, Key{ 1, "END" }, Key{ 1, "PAGE_DOWN" },
			padding, innerEdge, padding,
			Key{ 1, "NUM_7" }, Key{ 1, "NUM_8" }, Key{ 1, "NUM_9" }, Key{ 1, "NUM_PLUS" },
			padding, outerEdge,
			dsubEdge),
		MakeRow("Row 3",
			outerEdge,
			Spacer{ Key{ 0.75 }.Width() },
			padding,
			Key{ 1, "H" }, Key{ 1, "J" }, Key{ 1, "K" }, Key{ 1, "L" }, Key{ 1, "SEMICOLON" }, Key{ 1, "QUOTE" }, Key{ 2.25, "ENTER" },
			padding, Spacer{ innerEdge.Width() * 2 + Key{ 1 }.Width() * 3 + padding.Width() * 2 }, padding,
			Key{ 1, "NUM_4" }, Key{ 1, "NUM_5" }, Key{ 1, "NUM_6" }, Key{ 1, "NUM_PLUS(2)" },
			padding, outerEdge,
			dsubEdge),
		MakeRow("Row 4",
			outerEdge,
			Spacer{ Key{ 1.25 }.Width() },
			padding,
			Key{ 1, "N" }, Key{ 1, "M" }, Key{ 1, "COMMA" }, Key{ 1, "PERIOD" }, Key{ 1, "FORWARD_SLASH" }, Key{ 2.75, "RIGHT_SHIFT" },
			padding, innerEdge, Spacer{ Key{ 1 }.Width() }, padding,
			Key{ 1, "UP" },
			padding, Spacer{ Key{ 1 }.Width() }, innerEdge, padding,
			Key{ 1, "NUM_1" }, Key{ 1, "NUM_2" }, Key{ 1, "NUM_3" }, Key{ 1, "NUM_ENTER" },
			padding, outerEdge,
			dsubEdge),
		MakeRow("Row 5",
			outerEdge, padding,
			Key{ 2.25, "RIGHT_SPACE" }, Key{ 1.75, "RIGHT_HYPER" }, Key{ 1.25, "RIGHT_ALT" }, Key{ 1.25, "RIGHT_WIN" }, Key{ 1.25, "RIGHT_MENU" }, Key{ 1.25, "RIGHT_CTRL" },
			padding, innerEdge, padding,
			Key{ 1, "LEFT" }, Key{ 1, "DOWN" }, Key{ 1, "RIGHT" },
			padding, innerEdge, padding,
			Key{ 2, "NUM_0" }, Key{ 1, "NUM_PERIOD" }, Key{ 1, "NUM_ENTER(2)" },
			padding, outerEdge,
			dsubEdge)
	);

	return 0;
}
