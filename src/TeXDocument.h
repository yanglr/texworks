/*
	This is part of TeXworks, an environment for working with TeX documents
	Copyright (C) 2007-08  Jonathan Kew

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	For links to further information, or to contact the author,
	see <http://tug.org/texworks/>.
*/

#ifndef TeXDocument_H
#define TeXDocument_H

#include <QMainWindow>
#include <QList>
#include <QRegExp>
#include <QProcess>

#include "ui_TeXDocument.h"

#include "FindDialog.h"

#include <hunspell.h>

class QAction;
class QMenu;
class QTextEdit;
class QToolBar;
class QLabel;
class QComboBox;
class QActionGroup;
class QTextCodec;

class TeXHighlighter;
class PDFDocument;

class TeXDocument : public QMainWindow, private Ui::TeXDocument
{
	Q_OBJECT

public:
	TeXDocument();
	TeXDocument(const QString &fileName, bool asTemplate = false);

	virtual ~TeXDocument();

	static TeXDocument *findDocument(const QString &fileName);
	static QList<TeXDocument*> documentList()
		{
			return docList;
		}
	static void openDocument(const QString &fileName, bool activate = true, int lineNo = 0, int selStart = -1, int selEnd = -1);


	TeXDocument *open(const QString &fileName);
	void makeUntitled();
	bool untitled()
		{ return isUntitled; }
	QString fileName() const
		{ return curFile; }
	QTextCursor textCursor()
		{ return textEdit->textCursor(); }
	QTextDocument* textDoc()
		{ return textEdit->document(); }
	QString getLineText(int lineNo) const;

signals:
	void syncFromSource(const QString&, int);
	void activatedWindow(QWidget*);

protected:
	void closeEvent(QCloseEvent *event);
	bool event(QEvent *event);

public slots:
	void selectWindow();
	void typeset();
	void interrupt();
	
private slots:
	void newFile();
	void newFromTemplate();
	void open();
	bool save();
	bool saveAs();
	void clear();
	void clipboardChanged();
	void doFontDialog();
	void doLineDialog();
	void doFindDialog();
	void doFindAgain(bool fromDialog = false);
	void doReplaceDialog();
	void doReplace(ReplaceDialog::DialogCode mode);
	void doIndent();
	void doUnindent();
	void doComment();
	void doUncomment();
	void toUppercase();
	void toLowercase();
	void balanceDelimiters();
	void setWrapLines(bool wrap);
	void setSyntaxColoring(bool coloring);
	void copyToFind();
	void copyToReplace();
	void findSelection();
	void showSelection();
	void pdfClosed();
	void updateRecentFileActions();
	void updateWindowMenu();
	void updateEngineList();
	void showCursorPosition();
	void editMenuAboutToShow();
	void processStandardOutput();
	void processError(QProcess::ProcessError error);
	void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void toggleConsoleVisibility();
	void acceptInputLine();
	void goToPreview();
	void syncClick(int lineNo);
	void openAt(QAction *action);
	void selectedEngine(QAction* engineAction);
	void selectedEngine(const QString& name);
	void contentsChanged(int position, int charsRemoved, int charsAdded);
	void setLanguage(const QString& lang);
	void hideFloatersUnlessThis(QWidget* currWindow);

private:
	void init();
	bool maybeSave();
	QTextCodec *scanForEncoding(const QString &peekStr, bool &hasMetadata, QString &reqName);
	void loadFile(const QString &fileName, bool asTemplate = false);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	void showPdfIfAvailable();
	void prefixLines(const QString &prefix);
	void unPrefixLines(const QString &prefix);
	void replaceSelection(const QString& newText);
	void zoomToLeft(QWidget *otherWindow);
	QTextCursor doSearch(const QString& searchText, const QRegExp *regex, QTextDocument::FindFlags flags, int rangeStart, int rangeEnd);
	void showConsole();
	void hideConsole();
	void goToLine(int lineNo, int selStart = -1, int selEnd = -1);
	void updateTypesettingAction();
	void findRootFilePath();
	void maybeCenterSelection(int oldScrollValue = -1);
	void showFloaters();
	void presentResults(const QList<SearchResult>& results);

	TeXHighlighter *highlighter;
	PDFDocument *pdfDoc;

	QTextCodec *codec;
	QString curFile;
	QString rootFilePath;
	bool isUntitled;

	QLabel *lineNumberLabel;

	QActionGroup *engineActions;
	QString engineName;

	QComboBox *engine;
	QProcess *process;

	QList<QAction*> recentFileActions;
	QMenu *menuRecent;

	Hunhandle *pHunspell;

	QList<QWidget*> latentVisibleWidgets;

	static QList<TeXDocument*> docList;
};

#endif