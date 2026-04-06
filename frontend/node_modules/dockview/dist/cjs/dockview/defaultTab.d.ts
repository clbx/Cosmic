import React from 'react';
import { IDockviewPanelHeaderProps } from 'dockview-core';
export type IDockviewDefaultTabProps = IDockviewPanelHeaderProps & React.HtmlHTMLAttributes<HTMLDivElement> & {
    hideClose?: boolean;
    closeActionOverride?: () => void;
};
export declare const DockviewDefaultTab: React.FunctionComponent<IDockviewDefaultTabProps>;
