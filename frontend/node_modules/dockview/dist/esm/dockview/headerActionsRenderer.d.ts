import React from 'react';
import { ReactPart, ReactPortalStore } from '../react';
import { DockviewApi, DockviewGroupPanel, DockviewGroupPanelApi, PanelUpdateEvent, IHeaderActionsRenderer, IDockviewHeaderActionsProps } from 'dockview-core';
export declare class ReactHeaderActionsRendererPart implements IHeaderActionsRenderer {
    private readonly component;
    private readonly reactPortalStore;
    private readonly _group;
    private readonly mutableDisposable;
    private readonly _element;
    private _part?;
    get element(): HTMLElement;
    get part(): ReactPart<IDockviewHeaderActionsProps> | undefined;
    constructor(component: React.FunctionComponent<IDockviewHeaderActionsProps>, reactPortalStore: ReactPortalStore, _group: DockviewGroupPanel);
    init(parameters: {
        containerApi: DockviewApi;
        api: DockviewGroupPanelApi;
    }): void;
    dispose(): void;
    update(event: PanelUpdateEvent): void;
    private updatePanels;
    private updateActivePanel;
    private updateGroupActive;
}
